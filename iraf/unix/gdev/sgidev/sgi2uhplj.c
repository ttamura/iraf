#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define import_spp
#define import_error
#include <iraf.h>

/*
 * SGI2UHPLJ.C -- Read IRAF SGI rasterfile from standard input, translate into
 * the Hewlett Packard Printer Command Language (HP Laserjet Series) and
 * write to standard output.
 *
 * Warning
 *	The output of this is for 150 dpi. At this resolution it will take
 *	some 200 seconds to plot an simple "prow".
 *
 * Usage
 *	sgi2uhplj.e [-params] [sgi_] [| lpr -Phpraw]
 *
 *	-l N   left edge; x plot origin in device pixels      def DEF_LEFT
 *	-b N   bottom edge; y plot origin in device pixels    def DEF_BOTTOM
 *	-w N   width of plot, device pixels starting from l   def DEF_WIDTH
 *	-h N   height of plot, device pixels starting from b  def DEF_HEIGHT
 *
 * Numeric values may be appended directly to their flags or separated by a
 * space; the input file name and the switches may occur in any order.
 */

#define OSOK		0		/* normal successful completion	   */
#define NBITS_CHAR	8		/* number of bits in a char        */	

/* Device opcodes and parameters.  The default edge and width parameters (DEF_) 
 * are given in HP pixels, at 150 dots/inch.  The HP plots in portrait mode
 * by default so RO must be set in GRAPHCAP. Thus the maximum page `width' is 
 * 11*150pixels, `height' is 8.5*150 pixels.
 */

#define DEV_INIT	"\033*t150R\033*r1A"	/* Enter graph.               */
#define DEV_END		"\033*rB\014"		/* Exit graph. */
#define DEV_VECT	"\033*p%03dX\033*p%03dY"/* x,y cursor posn in dots */
#define DEV_RAST	"\033*b%03dW"		/* transfer raster graphics */

#define DEF_LEFT	15	/* origin in device pixels in x		      */
#define DEF_WIDTH	1216	/* width in x (150d/i, 8.5" paper)	      */
#define DEF_BOTTOM	30	/* origin in device pixels in y		      */
#define DEF_HEIGHT	1590	/* height in y (150d/i, 11" paper)	      */

#define SZ_VECT		14	/* total chars in cursor position command   */
#define SZ_RAST		7	/* total chars in transfer graphics command   */

/* graphcap entry for uhplj and sgi_hplaserjet. one problem with current
 * entry is that graph comes out slightly to the right of center on the page
 * The printer used, "hpraw", is site dependent.
 *
 * uhplj|UNIX generic interface to Hewlett-Packard LaserJet II:\
 *	:BF:WS:XO#0:YO#0:LO#2:LS#2:\
 *	:DD=plnode!hpii,tmp$sgk,!{ sgidispatch sgi2uhpii $F \
 *	-l$(XO) -b$(YO) -w$(PX) -h$(PY) $F | lpr -Phpraw; rm $F; }&:\
 *	:tc=sgi_hplaserjet:
 *
 * sgi_hplaserjet|Hewlett Packard LaserJet Plus at 150 dpi:\
 *	:kf=bin$x_sgikern.e:tn=sgikern:cw#.0125:ch#.0294:\
 *	:ar#1.325:xs#.2032:ys#.2692:xr#1200:yr#1590:\
 *	:XO#8:YO#0:XW#1200:YW#1590:PX#1216:PY#1590:LO#1:LS#0:\
 *	:BI:MF#8:RO:NB#8:
 */

static int dev_left;
static int dev_bottom;
static int dev_width;
static int dev_height;

static int get_iarg ( char, char *const [], int, int );
static void translate ( FILE *, FILE * );
static const char *xyencode ( int, int );
static void bswap2 ( const char *, char *, int );
static int swapped ( void );

/* MAIN -- Main entry point for SGI2UHPII.  Optional arguments are device
 * window parameters and name of input file.
 */
int main ( int argc, char *argv[] )
{
	const char *infile;
	const char *argp;
	int argno;
	FILE *in;

	infile = "stdin";

	/* Process the command line.
	 */
	for (argno=1;  (argp = argv[argno]) != NULL;  argno++) {
	    if (argp[0] == '-') {
		/* A window-control or pen width switch.
		 */
		switch (argp[1]) {
		case 'l':
		    dev_left = get_iarg (argp[2], argv, argno, DEF_LEFT);
		    break;
		case 'b':
		    dev_bottom = get_iarg (argp[2], argv, argno, DEF_BOTTOM);
		    break;
		case 'w':
		    dev_width = get_iarg (argp[2], argv, argno, DEF_WIDTH);
		    break;
		case 'h':
		    dev_height = get_iarg (argp[2], argv, argno, DEF_HEIGHT);
		    break;
		default:
		    break;
		}

	    } else {
		/* Input sgi-raster file specification.
		 */
		infile = argp;
	    }
	}

	if (strcmp (infile, "stdin") == 0)
	    in = stdin;
	else
	    in = fopen (infile, "r");

	if (in == NULL) {
	    fprintf (stderr, "Fatal error (sgi2uhpii):  Cannot open `%s'\n",
		infile);
	    fflush (stderr);
	    exit (OSOK+1);
	}

	/* Process the rasterfile.
	 */
	translate (in, stdout);

	if (in != stdin)
	    fclose (in);

	return 0;
}


/* GET_IARG -- Get an integer argument, whether appended directly to flag
 * or separated by a whitespace character; if error, report and assign default.
 */

static int get_iarg ( char argp, char *const argv[], int argno, int def_val )
{
	int	temp_val;

	if (argp == '\0')
	    if (argv[argno+1] == NULL) {
		fprintf (stderr, "missing arg to switch `%c';", argp);
		fprintf (stderr, " reset to %d\n", def_val);
		temp_val = def_val;
	    } else
		temp_val = atoi (argv[++argno]);
	else
	    temp_val = atoi (argv[argno]+2);

	return (temp_val);
}


/* TRANSLATE -- Interpret input SGI Raster File format into Hewlett Packard
 * Raster graphics instructions and write to stdout.
 */
static void translate ( FILE *in, FILE *out )
{
	char buf_rast [SZ_RAST];
	unsigned char *bp1, *buffer1;
	int n1, swap_bytes;
	int n, nlines, length, len_buf;

	swap_bytes = swapped ();

	len_buf = dev_width / NBITS_CHAR;
	if ( len_buf <= 0 ) return;
	buffer1 = (unsigned char *)malloc (len_buf);
	if ( buffer1 == NULL ) {
	    fprintf (stderr, "Fatal error :  malloc() failed\n");
	    fflush (stderr);
	    exit (OSOK+1);
	}

	/* Output device initialization.
	 */
	fwrite (xyencode (dev_left, dev_bottom), SZ_VECT, 1, out);
	fwrite (DEV_INIT, strlen(DEV_INIT), 1, out);

	/* Process the raster file
	 */
	nlines = dev_height;
	while ((n = fread (buffer1, len_buf, 1, in)) > 0) {

	    if (swap_bytes) 
		bswap2 (buffer1, buffer1, len_buf);

	    /* Keep track of number of lines left on the page.
	     */
	    if (!(nlines--)) {
		nlines += dev_height;
		fwrite (DEV_END, strlen (DEV_END), 1, out);
		fwrite (xyencode (dev_left, dev_bottom), SZ_VECT, 1, out);
		fwrite (DEV_INIT, strlen (DEV_INIT), 1, out);
	    }

	    /* Search for trailing null bytes to trim them off.
	     */
	    length = len_buf;
	    for (bp1 = buffer1+length; length && *(--bp1) == 0; length--)
		;

	    n1 = length;
	    if (n1 == 0) {
		n1 = 1;
		*buffer1 = 0;
	    }

	    /* Now copy out this line and prefix it with the control codes.
	     */
	    snprintf (buf_rast, SZ_RAST, DEV_RAST, n1);
	    fwrite (buf_rast, SZ_RAST, 1, out);
	    fwrite (buffer1, n1, 1, out);
	}

	/* Terminate plotting and exit.
	 */
	fwrite (DEV_END, strlen(DEV_END), 1, out);

	free(buffer1);
}

/* XYENCODE -- Encode x, y into a character string formatted for the device.
 */

static const char *xyencode ( int x, int y )
{
    static char obuf [SZ_VECT];

    snprintf (obuf, SZ_VECT, DEV_VECT, x, y);
    return (obuf);
}

/* BSWAP2 -- Move bytes from array "a" to array "b", swapping successive
 * pairs of bytes.  The two arrays may be the same but may not be offset
 * and overlapping.
 */
/* a      : input array			*/
/* b      : output array		*/
/* nbytes : number of bytes to swap	*/
static void bswap2 ( const char *a, char *b, int nbytes )
{
	const char *ip;
	char *op, *otop;
	unsigned int temp;

	ip = a;
	op = b;
	otop = op + (nbytes & ~1);

	/* Swap successive pairs of bytes.
	 */
	while (op < otop) {
	    temp  = *ip++;
	    *op++ = *ip++;
	    *op++ = temp;
	}

	/* If there is an odd byte left, move it to the output array.
	 */
	if (nbytes & 1)
	    *op = *ip;
}


/* SWAPPED -- Test whether we are running on a byte-swapped machine.
 */
static int swapped( void )
{
	union {
	    short   tswap;
	    char    b[2];
	} test;

	test.tswap = 1;
	return (test.b[0]);
}	


