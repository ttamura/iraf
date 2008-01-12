/* Copyright(c) 1986 Association of Universities for Research in Astronomy Inc.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define import_kernel
#define import_knames
#define import_protect
#define import_spp
#include <iraf.h>

#include "zos.h"

#define ZP_PREFIX	".."		/* hidden link for protected files */

static int chk_prot ( const char *, const char * );

/* ZFPROT -- Protect a file from accidental deletion.  In UNIX, this is
 * done by making another link to the file.  If ZFPROT is directed to protect
 * a file, and the file is already protected, the call is ignored.  Similarly,
 * if ZFPROT is directed to remove protection, and the file is not protected,
 * the call is ignored.
 */
int ZFPROT ( PKCHAR *fname, XINT *action, XINT *status )
{
	char link_name[SZ_PATHNAME];
	char *p;
	int first;

	/* Build up name of link file: "dir/..fname".  This is done by copying
	 * fname to the filename buffer of the link file and truncating the
	 * new filename after the directory prefix (if any).
	 */
	safe_strcpy (link_name, SZ_PATHNAME, (const char *)fname);
	if ((p = strrchr (link_name, '/')) != NULL) {
	    *(p+1) = EOS;
	    first = p - link_name + 1;		/* first char after '/' */
	} else {
	    *link_name = EOS;
	    first = 0;
	}

	safe_strcat (link_name, SZ_PATHNAME, ZP_PREFIX);
	safe_strcat (link_name, SZ_PATHNAME, &((const char *)fname)[first]);

	if (access ((const char *)fname, 0) == ERR) {
	    *status = XERR;
	    return *status;
	}

	switch (*action) {
	case REMOVE_PROTECTION:
	    if (chk_prot ((const char *)fname, link_name) == XNO)
		*status = XOK;
	    else if (unlink (link_name) == ERR)
		*status = XERR;
	    else 
		*status = XOK;
	    return *status;

	case SET_PROTECTION:
	    *status = XOK;
	    if (chk_prot ((const char *)fname, link_name) == XNO) {
		unlink (link_name);
		if (link ((const char *)fname, link_name) == ERR)
		    *status = XERR;
	    }
	    return *status;

	default:
	    *status = chk_prot ((const char *)fname, link_name);
	    return *status;
	}
}


/* CHK_PROT -- Determine whether or not a file is protected.
 */
static int chk_prot ( const char *fname, const char *link_name )
{
	struct	stat file1, file2;

	if (access(link_name,0) == ERR)
	    return (XNO);
	else {
	    stat (fname, &file1);
	    stat (link_name, &file2);
	    /* Make sure prefixed file is actually a link to original file */
	    if (file1.st_ino == file2.st_ino && file1.st_dev == file2.st_dev)
		return (XYES);
	    else
		return (XNO);
	}
}
