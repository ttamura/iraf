/* Copyright(c) 1986 Association of Universities for Research in Astronomy Inc.
 */

#define import_spp
#define import_knames
#include <iraf.h>

/* ACHTB_ -- Unpack an unsigned byte array into an SPP array.
 * The loop runs in the reverse direction so that the unpack can be
 * performed in place (a and b can be the same array).
 */
int ACHTBR ( XCHAR *a, XREAL *b, XINT *npix )
{
	XUBYTE *ip, *first = (XUBYTE *)a;
	XREAL *op;

	for (ip = &first[*npix], op = &((XREAL *)b)[*npix];  ip > first;  )
		*--op = *--ip;

	return 0;
}
