include	<imhdr.h>
include "ms.h"

# T_NEW_IMAGE -- General MULTISPEC extraction task.
#
# The general task parameters are obtained and the desired extraction
# procedure is called.  The input database and image are accessed and
# the output image is created.

procedure t_new_image ()

# User parameters:
char	image[SZ_FNAME]			# MULTISPEC database
char	output[SZ_FNAME]		# Output image file
real	lower				# Lower limit of strip
real	upper				# Upper limit of strip
int	lines[3, MAX_RANGES]		# Lines to be extracted
int	spectra[3, MAX_RANGES]		# Spectra to be extracted
bool	ex_model			# Extract model or data
bool	clean				# Correct for bad pixels
int	nreplace			# Maximum number of bad pixels replaced
real	sigma_cut			# Threshold for replacing bad pixels
int	model				# Model type: gauss5, profile

bool	ex_spectra			# Extract spectra or image line
bool	integrated			# Extract integrated spectra or strip
int	nlines
pointer	ms, im_in, im_out

int	clgeti(), ms_model_id(), clgranges()
bool	clgetb()
real	clgetr()
pointer	msmap(), immap()

begin
	# Access input and output files.
	call clgstr ("image", image, SZ_FNAME)
	ms = msmap (image, READ_ONLY, 0)
	im_in = immap (image, READ_ONLY, 0)
	call clgstr ("output", output, SZ_FNAME)
	im_out = immap (output, NEW_IMAGE, 0)

	# Determine extraction limits.
	nlines = clgranges ("lines", 1, IM_LEN(im_in, 2), lines, MAX_RANGES)
	lower = clgetr ("lower")
	upper = clgetr ("upper")

	# Determine type of extraction.
	ex_spectra = FALSE
	ex_model = clgetb ("ex_model")
	integrated = FALSE

	# Determine whether to clean data lines and the cleaning parameters.
	clean = clgetb ("clean")
	if (clean) {
	    nreplace = clgeti ("nreplace")
	    sigma_cut = clgetr ("sigma_cut")
	} else
	    nreplace = 0

	# Set type of model to be used.
	model = NONE
	if (ex_model || clean)
	    model = ms_model_id ("model")

	# Set verbose output.
	call ex_set_verbose (clgetb ("verbose"))
	call ex_prnt1 (image, output)

	# Set image header for output extraction image file.
	IM_NDIM(im_out) = IM_NDIM(im_in)
	IM_LEN(im_out, 1) = IM_LEN(im_in, 1)
	IM_LEN(im_out, 2) = nlines
	IM_PIXTYPE(im_out) = IM_PIXTYPE(im_in)
	call strcpy (IM_TITLE(im_in), IM_TITLE(im_out), SZ_IMTITLE)

	# Select extraction procedure based on model.
	switch (model) {
	case GAUSS5:
	    call set_fit_and_clean (clgeti ("niterate"), nreplace,
		sigma_cut, clgeti ("fit_type"), ex_model)
	    call ex_gauss5 (ms, im_in, im_out, spectra, lines, lower, upper,
		ex_spectra, ex_model, integrated)
	case SMOOTH:
	    call set_fit_smooth (nreplace, sigma_cut)
	    call ex_smooth (ms, im_in, im_out, spectra, lines, lower, upper,
		ex_spectra, ex_model, integrated)
	default:
	    call ex_strip (ms, im_in, im_out, spectra, lines, lower, upper,
		ex_spectra, ex_model, integrated)
	}

	# Close files.
	call imunmap (im_in)
	call imunmap (im_out)
	call msunmap (ms)
end
