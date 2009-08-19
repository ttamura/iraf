include <mach.h>
include "../lib/center.h"

# AP_MCTR1D -- Procedure to compute the center from the 1D marginal
# distributions.

int procedure ap_mctr1d (ctrpix, nx, ny, norm, xc, yc, xerr, yerr)

real	ctrpix[nx, ny]		# object to be centered
size_t	nx, ny			# dimensions of subarray
real	norm			# the normalization factor
real	xc, yc			# computed centers
real	xerr, yerr		# estimate of centering error

pointer	sp, xm, ym

begin
	call smark (sp)
	call salloc (xm, nx, TY_REAL)
	call salloc (ym, ny, TY_REAL)

	# Compute the marginal distributions.
	call ap_mkmarg (ctrpix, Memr[xm], Memr[ym], nx, ny)
	call adivkr (Memr[xm], real (ny), Memr[xm], nx)
	call adivkr (Memr[ym], real (nx), Memr[ym], ny)

	# Get the centers and errors.
	call ap_cmmarg (Memr[xm], nx, norm, xc, xerr)
	call ap_cmmarg (Memr[ym], ny, norm, yc, yerr)

	call sfree (sp)
	return (AP_OK)
end


# AP_CMMARG -- Compute the center and estimate its error given the
# marginal distribution and the number of points. The center is estimated
# using a centroid which is an optimal solution for a gaussian star with
# no background noise. The centering error is computing using a variance
# estimate based on a gaussian star with no background noise.

procedure ap_cmmarg (a, npts, norm, xc, err)

real	a[npts]		# array
size_t	npts		# number of points
real	norm		# the normalization factor
real	xc		# center value
real	err		# error

long	i, npos
real	sumi, sumix, sumix2, mean, val
real	asumr()

begin
	# Initialize.
	mean = asumr (a, npts) / npts
	npos = 0
	sumi = 0.0
	sumix = 0.0
	sumix2 = 0.0

	# Accumulate the sums.
	do i = 1, npts {
	    val = (a[i] - mean)
	    if (val > 0.0) {
		npos = npos + 1
	        sumi = sumi + val
	        sumix = sumix + val * i
	        sumix2 = sumix2 + val * i ** 2
	    }
	}

	# Compute the position and the error.
	if (npos <= 0) {
	    xc =  (1.0 + npts) / 2.0
	    err = INDEFR
	} else {
	    xc = sumix / sumi
	    err = (sumix2 / sumi - xc * xc)
	    if (err <= 0.0) {
		err = 0.0
	    } else {
	        err = sqrt (err / ((sumi + mean * npts) * norm))
		if (err > real (npts))
		    err = INDEFR
	    }
	}
end
