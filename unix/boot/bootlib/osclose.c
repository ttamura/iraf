/* Copyright(c) 1986 Association of Universities for Research in Astronomy Inc.
 */

#include "bootlib.h"

/* OS_CLOSE -- Close a file created (opened) by OSCREATE.  If writing to a
 * text file flush any incomplete (non newline terminated) output line.
 */
os_close (fd)
int	fd;
{
	int	junk;
	XINT	nchars;

	if (osfiletype == BINARY_FILE)
	    close (fd);
	else {
	    if (txop > text) {
		nchars = txop - text;
		ZPUTTX (&fd, text, &nchars, &junk);
	    }
	    ZCLSTX (&fd, &junk);
	}
}
