include <mach.h>
include "../lib/apphotdef.h"
include "../lib/noisedef.h"
include "../lib/photdef.h"
include "../lib/phot.h"

# AP_WREMAG -- Procedure to recompute the magnitudes inside a set of apertures
# given that the sums and effective areas have already been computed.

int procedure ap_wremag (ap, positive, skyval, skysig, nsky)

pointer	ap		# pointer to the apphot structure
int	positive	# emission or absorption features
real	skyval		# sky value
real	skysig		# sigma of sky
int	nsky		# number of sky pixels

pointer	nse, phot
real	zmag

begin
	# Initalize.
	phot = AP_PPHOT(ap)
	nse = AP_NOISE(ap)
	call amovkr (INDEFR, Memr[AP_MAGS(phot)], AP_NAPERTS(phot)]
	call amovkr (INDEFR, Memr[AP_MAGERRS(phot)], AP_NAPERTS(phot)]

	if (IS_INDEFR(AP_PXCUR(phot)) || IS_INDEFR(AP_PYCUR(phot)))
	    return (AP_APERT_NOAPERT)
	if (IS_INDEFR(skyval))
	    return (AP_APERT_NOSKYMODE)

	# Compute the magnitudes and errors.
	if (positive == YES)
	    call apcopmags (Memr[AP_SUMS(phot)], Memr[AP_AREA(phot)],
	        Memr[AP_MAGS(phot)], Memr[AP_MAGERRS(phot)], AP_NMAXAP(phot),
	        skyval, skysig, nsky, AP_ZMAG(phot), AP_NOISEFUNCTION(nse),
		AP_EPADU(nse))
	else
	    call apconmags (Memr[AP_SUMS(phot)], Memr[AP_AREA(phot)],
	        Memr[AP_MAGS(phot)], Memr[AP_MAGERRS(phot)], AP_NMAXAP(phot),
	        skyval, skysig, nsky, AP_ZMAG(phot), AP_NOISEFUNCTION(nse),
		AP_EPADU(nse), AP_READNOISE(nse))

	# Correct for itime.
	zmag = 2.5 * log10 (AP_ITIME(ap))
	call aaddkr (Memr[AP_MAGS(phot)], zmag, Memr[AP_MAGS(phot)],
	    AP_NAPERTS(phot)]

	if (AP_NMINAP(phot) <= AP_NMAXAP(phot))
	    return (AP_APERT_BADDATA)
	else if (AP_NMAXAP(phot) < AP_NAPERTS(phot))
	    return (AP_APERT_OUTOFBOUNDS)
	else
	    return (AP_OK)
end
