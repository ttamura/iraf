# Definitions for SKYBLOCK algorithm.

define	SKBSKYMIN	10000	# Minimum number of sky pixels in subblock
define	SKBFRAC		0.66	# Frac of sky pix in subblock for sky estimate
define	SKBGROW		1.5	# Grow for rejected subblocks
define	SKBSIGBIN	2.5	# Sigma width of histogram
define	SKBA		0.01	# Histogram resolution
define	SKBNMINPIX	50	# Minimum number of pixels/subblock/side
define	SKBNMINBINS	500	# Minimum average bin population
define	SKBHISTWT	2	# Default histogram weighting power
define	SKBCNV		""	# Convolution

define	SKB_STRLEN		99	# String length in parameters
define	SKB_LEN			82	# Sky block structure length

# The following apply to all images.
define	SKB_BLKSTEP	Memi[P2I($1)]	# Step size
define	SKB_BLKSIZE	Memi[P2I($1+1)]	# Number of pixels or blocks
define	SKB_NSUBBLKS	Memi[P2I($1+2)]	# Number of subblocks per block
define	SKB_SKYMIN	Memi[P2I($1+3)]	# Minimum number of sky pixels
define	SKB_NMINPIX	Memi[P2I($1+4)]	# Min pixels/subblock/side
define	SKB_SIGBIN	Memr[P2R($1+5)]	# Histogram sigma limit
define	SKB_A		Memr[P2R($1+6)]	# Histogram resolution
define	SKB_B		Memr[P2R($1+7)]	# Bin start
define	SKB_NBINS	Memi[P2I($1+8)]	# Number of bins
define	SKB_NMINBINS	Memi[P2I($1+9)]	# Min avg bin population
define	SKB_HISTWT	Memi[P2I($1+10)]	# Histogram weighting power
define	SKB_SIGFAC	Memr[P2R($1+11)]	# Sigma correction factor
define	SKB_FRAC	Memr[P2R($1+12)]	# Frac of sky pix in subblock
define	SKB_GROW	Memr[P2R($1+13)]	# Grow for rejected subblocks
define	SKB_CNV		P2C($1+14)	# Pointer to convolution string (99)

# The following are set for each image.
define	SKB_NCBLK	Memi[P2I($1+65)]	# Number of blocks across image
define	SKB_NLBLK	Memi[P2I($1+66)]	# Number of blocks across image
define	SKB_NCPIX	Memi[P2I($1+67)]	# Number of pixels in blocks
define	SKB_NLPIX	Memi[P2I($1+68)]	# Number of pixels in blocks
define	SKB_NCSBLK	Memi[P2I($1+69)]	# Number of subblocks across image
define	SKB_NLSBLK	Memi[P2I($1+70)]	# Number of subblocks across image
define	SKB_NCSPIX	Memi[P2I($1+71)]	# Number of pixels in subblocks
define	SKB_NLSPIX	Memi[P2I($1+72)]	# Number of pixels in subblocks
define	SKB_NSKYMIN	Memi[P2I($1+73)]	# Minimum pixels to evaluate histogram
define	SKB_BINS	Memi[P2I($1+74)]	# Pointer to bins
define	SKB_NAV		Memi[P2I($1+75)]	# Number of bins to average for weights
define	SKB_NSKY	Memi[P2I($1+76)]	# Pointer to num sky pix
define	SKB_EXP		Memi[P2I($1+77)]	# Pointer to exposure values
define	SKB_SKYS	Memi[P2I($1+78)]	# Pointer to sky block values
define	SKB_SIGS	Memi[P2I($1+79)]	# Pointer to sigma block values
define	SKB_SKY		Memi[P2I($1+80)]	# Pointer to current sky block line
define	SKB_SIG		Memi[P2I($1+81)]	# Pointer to current sigma block line
