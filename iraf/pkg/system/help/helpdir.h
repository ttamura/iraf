# Size limiting definitions.

define	MAX_LDIRS	50			# maximum ldirs for package
define	MAX_MODULES	100			# initial maximum no. modules
define	INC_MODULES	50			# increment if overflow
define	SZ_SBUF		2048			# initial size string buffer
define	INC_SZSBUF	1024			# increment if overflow

define	LEN_HDSTRUCT	(10+MAX_LDIRS+MAX_MODULES*LEN_MODSTRUCT)
define	LEN_BASEHD	10
define	LEN_MODSTRUCT	6

# Helpdir descriptor structure.

define	HD_SBUF		Memi[P2I($1)]		# string buffer
define	HD_NEXTCH	Memi[P2I($1+1)]		# index of next char in sbuf
define	HD_SZSBUF	Memi[P2I($1+2)]		# size of string buffer
define	HD_DEFDIR	Memi[P2I($1+3)]		# offset to defdir
define	HD_NLDIRS	Memi[P2I($1+4)]		# number of logical directories
define	HD_NMODULES	Memi[P2I($1+5)]		# number of modules in package
define	HD_MAXMODULES	Memi[P2I($1+6)]		# max no. of modules in package
define	HD_LENHD	Memi[P2I($1+7)]		# length of this structure
define	HD_PAKNAME	Memi[P2I($1+8)]		# offset of package name string
define	HD_LDIR		Memi[P2I($1+10+$2-1)]	# indices of ldir strings
define	HD_MODULE	($1+60+($2-1)*6)	# module descriptors

# Module descriptor structure.

define	M_NAME		Memi[P2I($1)]		# module name index
define	M_HLP		Memi[P2I($1+1)]		# help file index
define	M_SYS		Memi[P2I($1+2)]		# system docs file index
define	M_SRC		Memi[P2I($1+3)]		# source code file index
define	M_PKG		Memi[P2I($1+4)]		# pkg helpdir file index
define	M_MEN		Memi[P2I($1+5)]		# package menu file index
