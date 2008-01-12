# Header file for GEOMAP

define	LEN_GEOMAP	(45 + SZ_FNAME + SZ_LINE + 2)

define	GM_XMIN		Memd[P2D($1)]		# Minimum x value
define	GM_XMAX		Memd[P2D($1+2)]		# Maximum x value
define	GM_YMIN		Memd[P2D($1+4)]		# Minimum y value
define	GM_YMAX		Memd[P2D($1+6)]		# Maximum y value
define	GM_XOREF	Memd[P2D($1+8)]		# Mean of xref coords
define	GM_YOREF	Memd[P2D($1+10)]	# Mean of yref coords
define	GM_XOIN		Memd[P2D($1+12)]	# Mean of x coords
define	GM_YOIN		Memd[P2D($1+14)]	# Mean of y coords
define	GM_XREFPT	Memd[P2D($1+16)]	# X reference point
define	GM_YREFPT	Memd[P2D($1+18)]	# Y reference point
define	GM_XRMS		Memd[P2D($1+20)]	# Rms of x fit
define	GM_YRMS		Memd[P2D($1+22)]	# Rms of y fit
define	GM_REJECT	Memd[P2D($1+24)]	# Sigma limit for rejection
define	GM_PROJECTION	Memi[P2I($1+26)]		# Coordinate projection type
define	GM_FIT		Memi[P2I($1+27)]		# Fit geometry type
define	GM_FUNCTION	Memi[P2I($1+28)]		# Function type
define	GM_XXORDER	Memi[P2I($1+29)]		# X fit X order
define	GM_XYORDER	Memi[P2I($1+30)]		# X fit Y order
define	GM_XXTERMS	Memi[P2I($1+31)]		# X fit cross-terms
define	GM_YXORDER	Memi[P2I($1+32)]		# Y fit X order
define	GM_YYORDER	Memi[P2I($1+33)]		# Y fit Y order
define	GM_YXTERMS	Memi[P2I($1+34)]		# Y fit cross-terms
define	GM_MAXITER	Memi[P2I($1+35)]		# maximum number of iterations
define	GM_NPTS		Memi[P2I($1+36)]		# Number of data points
define	GM_NREJECT	Memi[P2I($1+37)]		# Number of rejected pixels
define	GM_NWTS0	Memi[P2I($1+38)]		# Number of pts with wts <= 0
define	GM_REJ		Memi[P2I($1+39)]		# Pointer to rejected pixels
define	GM_RECORD	Memc[P2C($1+40)]	# Record name
define	GM_PROJSTR	Memc[P2C($1+40+SZ_FNAME+1)]	# Projection parameters

# define the permitted coordinate projections

define  GM_PROJLIST      "|lin|azp|tan|sin|stg|arc|zpn|zea|air|cyp|car|\
mer|cea|cop|cod|coe|coo|bon|pco|gls|par|ait|mol|csc|qsc|tsc|tnx|zpx|"

define	GM_NONE	     0
define  GM_LIN	     1
define  GM_AZP       2
define  GM_TAN       3
define  GM_SIN       4
define  GM_STG       5
define  GM_ARC       6
define  GM_ZPN       7
define  GM_ZEA       8
define  GM_AIR       9
define  GM_CYP       10
define  GM_CAR       11
define  GM_MER       12
define  GM_CEA       13
define  GM_COP       14
define  GM_COD       15
define  GM_COE       16
define  GM_COO       17
define  GM_BON       18
define  GM_PCO       19
define  GM_GLS       20
define  GM_PAR       21
define  GM_AIT       22
define  GM_MOL       23
define  GM_CSC       24
define  GM_QSC       25
define  GM_TSC       26
define  GM_TNX       27
define  GM_ZPX       28

# define the permitted fitting geometries 

define	GM_GEOMETRIES	"|shift|xyscale|rotate|rscale|rxyscale|general|"

define	GM_SHIFT		1
define	GM_XYSCALE		2
define	GM_ROTATE		3
define	GM_RSCALE		4
define	GM_RXYSCALE		5
define	GM_GENERAL		6

# define the permitted fitting functions

define	GM_FUNCS	"|chebyshev|legendre|polynomial|"

# define the permitted x-terms functions

define	GM_XFUNCS	"|none|full|half|"
