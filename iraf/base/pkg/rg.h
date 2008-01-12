# Definitions for the RANGES structure.

define	LEN_RG		2

define	RG_NPTS		Memi[P2I($1)]		# Number of points in ranges
define	RG_NRGS		Memi[P2I($1+1)]		# Number of range intervals
define	RG_X1		Memi[P2I($1+2*($2))]		# Start of interval $2
define	RG_X2		Memi[P2I($1+2*($2)+1)]	# End of interval $2
