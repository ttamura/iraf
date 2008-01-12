/* Copyright(c) 1986 Association of Universities for Research in Astronomy Inc.
 */

#define	import_stdio
#define	import_libc
#define	import_ctype
#define	import_fset
#define	import_spp
#include <iraf.h>

#include "config.h"
#include "operand.h"
#include "param.h"
#include "task.h"
#include "eparam.h"


#define	COLWIDTH	40		/* column width for showhelp	*/


/*-------------------------------------------------------------------------
 * EDCAP.C -- Tools to support the edcap utility, used to define the input
 * language of screen editors.
 *
 * External procedures:
 *    edtinit		initialize the editor database and terminal
 *    edtexit		terminate edit mode (may send output to terminal)
 *    host_editor	return host command used to call the named editor
 *    what_cmd		convert escape sequence into editor command
 *    show_editorhelp	print a list of editor keystrokes
 *
 * Internal procedures:
 *    get_editor	open and scan the EDCAP file
 *    cmd_match		search the editor command list for an escape sequence
 *    map_escapes	map encoded escape sequence from EDCAP file
 *
 *-------------------------------------------------------------------------
 */

static char ed_editorcmd[SZ_LINE+1];

static void get_editor ( const char * );
static int cmd_match ( const char *, int );
static void map_escapes ( const char *, char *, int );


/* EDTINIT -- Initialize the editor.
 */
void edtinit( void )
{
	register int	i;
	char 	editor[SZ_FNAME];	/* the name of the editor	 */

	/* See if the current editor is the one to use.  If not, get the
	 * editor.ed definitions.
	 */
	if (c_envgets ("editor", editor, SZ_FNAME) > 0)
	    if (strcmp (editor, command[EDITOR_ID].keystroke) != 0)
		get_editor (editor);

	/* Count the number of editor commands.
	 */
	numcommands = FIRST_CMD;
	for (i=FIRST_CMD;  command[i].cmd < NOMORE_COMMANDS;  i++)
	    numcommands++;

	/* Send the initial edit sequence (to enable keypad, if any).
	 */
	if (*(command[EDIT_INIT].escape) != '\0')
	    printf ("%s",command[EDIT_INIT].escape); 

	/* Enable transmission of the screen repaint sequence, to be returned
	 * by the terminal driver if the process is suspended and later
	 * resumed while in raw mode.
	 */
	for (i=FIRST_CMD;  command[i].cmd < NOMORE_COMMANDS;  i++)
	    if (command[i].cmd == REPAINT && strlen(command[i].escape)==1)
		c_fseti (STDOUT, F_SETREDRAW, command[i].escape[0]);
}


/* EDTEXIT -- Terminate the editor.  Send an escape sequence to the terminal
 * if necessary.
 */
void edtexit( void )
{
	c_fseti (STDOUT, F_SETREDRAW, 0);
	if (*(command[EDIT_TERM].escape) != '\0')
	    printf ("%s",command[EDIT_TERM].escape); 
}


/* HOST_EDITOR -- Return a pointer to the command string to be sent to the
 * host system to run an editor, given the user name for the editor.
 */
char *host_editor ( const char *editor )
{
	get_editor (editor);
	return (ed_editorcmd);
}


/* GET_EDITOR -- Redefine the editor keystrokes from the editor.ed file.
 * Search for that file first in the users home directory.  If not found
 * there, look in the standard device directory.
 */
/* editor : the name of the editor		*/
static void get_editor ( const char *editor )
{
	FILE	*fp;		/* pointer to the editor.ed file	*/
	char	string[SZ_LINE];/* an edcap string from the .ed file	*/
	char	label[SZ_LINE];	/* the key-sequence label (keyword)	*/
	char	escape[SZ_LINE];/* the escape sequence in c octal	*/
	char	name[SZ_LINE];	/* the keystroke name, for HELP		*/
	char	fname[SZ_PATHNAME];
	int	i, num, n;

	/* Search the directories for the edcap file editor.ed.
	 */
	snprintf (fname, SZ_PATHNAME, "home$%s.ed", editor);
	fp = fopen (fname, "r");

	if (fp == NULL) { 
	    snprintf (fname, SZ_PATHNAME, "dev$%s.ed", editor);
	    fp = fopen (fname, "r");

	    if (fp == NULL) {
		eprintf ("cannot find edcap file for `%s'\n", editor);
		eprintf ("editor language defaults to `%s'\n",
		    command[EDITOR_ID].keystroke);
		return;
	    }
	}

	/* Parse the edcap file and initialize the command list and the host
	 * editor command string (default `irafvi', `irafemacs', etc.).
	 */
	snprintf (ed_editorcmd, SZ_LINE+1, "iraf%s", editor);
	num = 0;

	while (fgets (string, SZ_LINE, fp) != NULL) {
	    /* Check for the EDITOR_CMD field, the command to be sent to the
	     * host system to run the editor.  This is a special case since
	     * the edcap format does not support anything but keystrokes.
	     * A termcap format file should have been used for this
	     * database, rather than defining a new format file, then this
	     * would not have been necessary.
	     */
	    if (strncmp (string, "EDITOR_CMD", 10) == 0) {
		char    *ip, *op;
		char    *maxop;
		char    delim;
		int	isformat;

		/* Extract the optionally quoted host command format string.
		 * This is either the editor command name (prefix), e.g.,
		 * "irafemacs", or an SPRINTF format string containing a %s
		 * where the filename(s) are to go.
		 */
		for (ip=string+10;  isspace(*ip);  ip++)
		    ;
		delim = (*ip == '"' || *ip == '\'') ? *ip++ : 0;
		isformat=NO;
		maxop = ed_editorcmd + SZ_LINE+1 -1;
		for ( op=ed_editorcmd ; op < maxop && (*ip) ; op++, ip++ ) {
		    *op = *ip;
		    if ((delim && *op == delim) || (!delim && isspace(*op)))
			break;
		    else if (*op == '%' && ip[1] == 's')
			isformat++;
		}

		/* If the command string did not contain an embedded %s to
		 * indicate where the file names(s) are to go, add one at
		 * the end, i.e., "... %s".
		 */
		if (!isformat) {
		    if ( op < maxop ) *op++ = ' ';
		    if ( op < maxop ) *op++ = '%';
		    if ( op < maxop ) *op++ = 's';
		}

		*op = EOS;
		continue;
	    }

	    /* Process a normal editor command into the command table.
	     * Each line must contain three tokens, the internal command
	     * name, the terminal escape sequence, and the keystroke name.
	     */
	    /* n = sscanf (string, "%s %s %s", label, escape, name); */
	    {
		char *ip;
		char *ip0;
		n = 0;
		for ( ip0 = string ; *ip0 == ' ' ; ip0++ );
		for ( ip = ip0 ; ; ip++ ) {
		    if ( *ip == ' ' || *ip == '\0' ) {
			int l = (ip-ip0)+1 < SZ_LINE ? (ip-ip0)+1 : SZ_LINE;
			if ( n == 0 ) {
			    strncpy(label,ip0,l);
			    label[l-1] = EOS;
			}
			else if ( n == 1 ) {
			    strncpy(escape,ip0,l);
			    escape[l-1] = EOS;
			}
			else if ( n == 2 ) {
			    strncpy(name,ip0,l);
			    name[l-1] = EOS;
			}
			n++;
			if ( *ip == '\0' || n == 3 ) break;
			if ( *ip == ' ' ) {
			    for ( ; ip[1] == ' ' ; ip++ );
			    ip0 = ip + 1;
			}
		    }
		}
	    }

	    if (n == 3) {
		/* Determine which legitimate editor command this is.
		 */
		for (i=0;  i < NUM_COMMANDS;  i++)
		    if (strcmp (label, cmdnames[i]) == 0)
			break;

		/* Stuff the command into the static command buffer.
		 */
		if (i < NUM_COMMANDS) {
		    command[num].cmd = i;
		    map_escapes (escape, label, SZ_LINE);
		    strncpy (command[num].escape, label, SZ_ESCAPE);
		    command[num].escape[SZ_ESCAPE] = EOS;
		    strncpy (command[num].keystroke, name, SZ_KEYSTROKE);
		    command[num].keystroke[SZ_KEYSTROKE] = EOS;
		    num++;
		}
	    }
	}

	/* Make sure the command buffer terminates here.
	 */
	command[num].cmd = NOMORE_COMMANDS;
	strcpy (command[num].escape, "");
	strcpy (command[num].keystroke, " ");

	strncpy (command[EDITOR_ID].keystroke, editor, SZ_KEYSTROKE);
	command[EDITOR_ID].keystroke[SZ_KEYSTROKE] = EOS;
	fclose (fp);
}


/* MAP_ESCAPES -- Take an ASCII string which may have escape sequences
 * encoded as octal (\nnn).  Copy the string to the output, replacing
 * the encoded values with the binary character value.  The output
 * string may be the same as the input string.
 *
 * Control codes may be represented in the input in any of the following ways:
 *
 *	^X		control-X
 *	\[befnrt]	backspace, escape, formfeed, newline, return, tab
 *	\nnn		octal constant
 *	\^		the character ^
 *	\\		the character \
 *
 * Ordinary characters are copied to the output.
 */
/* input   : pointer into input string	*/
/* output  : pointer into output string	*/
/* bufsize : buffer size of output	*/
static void map_escapes ( const char *input, char *output, int bufsize )
{
	static const char *echars = "befnrt";
	static const char *ecodes = "\b\033\f\n\r\t";
	register const char *ip = input;
	register char	*op = output;
	char *maxop = output + bufsize -1;
	register int	n;

	while (*ip != '\0') {
	    if (*ip == '\\') { 
		switch (*++ip) {
		case 'b': case 'e': case 'f':
		case 'n': case 'r': case 't':
		    if ( op < maxop ) 
			*op++ = ecodes[index(echars,*ip++)-echars];
		    break;
		default:
		    if (isdigit (*ip)) {
			for (n=0;  isdigit(*ip) != 0;  ip++) {
			    n = n * 8 + (*ip - '0');
			}
			if ( op < maxop ) *op++ = n;
		    } else {
			if ( op < maxop ) *op++ = *ip++;
		    }
		}
	    } else if (*ip == '^') {
		ip++;
		if ( op < maxop ) *op++ = (*ip++ % 040);
	    } else {
		if ( op < maxop ) *op++ = *ip++;
	    }
	}

	if ( op <= maxop ) *op = '\0';
}


/* WHAT_CMD -- Determine which editing command has been sent.  Such commands
 * must begin with a non-printable character.  Return the command number or
 * zero if unrecognized.  We are called with the first character of the
 * command (some control code).  Additional keystrokes are read from the
 * standard input until an editor command is recognized.
 */
/* first_char : the first unprintable character */
int what_cmd ( char first_char )
{
	register int	nchars, k;
	char	cmd_string[9];
	char	*cmd;

	cmd = cmd_string;
	*cmd = first_char;

	/* Loop until we get an exact match or until we get no match.
	 * A character is read from the standard input in each pass
	 * through the loop.
	 */
	for (nchars=1;  nchars < 9;  nchars++)
	    if ((k = cmd_match (cmd_string, nchars)) < 0)
		return (0);
	    else if (nchars == strlen (command[k].escape))
		return (command[k].cmd);
	    else
		*(++cmd) = fgetc(stdin);

	return (0);
}


/* CMD_MATCH -- Scan the first nchars of the available commands to see if
 * any match the command string.  Return -1 if the command string does not
 * match any editor escape sequence, else return the index of the first
 * command code matched.
 */
/* cstring : command string	*/
/* nchars  : nchars to compare	*/
static int cmd_match ( const char *cstring, int nchars )
{
	int	k;

	for (k=FIRST_CMD;  k <= numcommands;  k++)
	    if (strncmp (cstring, command[k].escape, nchars) == 0)
		return (k);

	return (-1);
}


/* SHOW_EDITORHELP -- Display the edit commands and their keystroke
 * equivalences.
 */
void show_editorhelp( void )
{
	char	sbuf[MAX_COMMANDS*COLWIDTH];
	char	*strp[MAX_COMMANDS];
	int	center, maxcols, firstcol, lastcol, nstrs, i;
	int	save_raw;

	maxcols = c_envgeti ("ttyncols");
	center = maxcols / 2;

	/* Disable raw mode output so that output processing will be enabled,
	 * e.g., to map newlines into crlfs.
	 */
	save_raw = c_fstati (STDOUT, F_RAW);
	c_fseti (STDOUT, F_RAW, NO);

	/* Format the help strings for the individual keystrokes.
	 */
	for (i=FIRST_CMD, nstrs=0;  i <= numcommands;  i++)
	    if (*(command[i].escape) != '\0') {
		strp[nstrs] = &sbuf[nstrs*COLWIDTH];
		snprintf (strp[nstrs], COLWIDTH, "%8w%-10.10s = %-11.11s%2w",
		    cmdnames[command[i].cmd], command[i].keystroke);
		nstrs++;
	    }

	e_clear();
	e_goto (center - 7, 1);
	e_putline ("EDIT COMMANDS (");
	e_putline (command[EDITOR_ID].keystroke);
	e_putline (")\n\n");

	/* Sort and output the string table.
	 */
	if (nstrs) {
	    strsort (strp, nstrs);
	    i = strlen (strp[0]);
	    firstcol = center - i - 2;
	    lastcol  = center + i + 2;
	    strtable (stdout, strp, nstrs, firstcol, lastcol, COLWIDTH, 2);
	}

	e_putline ("\n");
	e_ctrl ("so");
	e_putline ("[hit any key to continue]");
	e_ctrl ("se");

	/* Restore raw mode.
	 */
	c_fseti (STDOUT, F_RAW, save_raw);

	fflush (stdout);

	/* Pause. */
	fgetc (stdin);
}
