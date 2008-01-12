# Copyright(c) 1986 Association of Universities for Research in Astronomy Inc.

# STREQ -- Compare two strings for equality.

bool procedure streq (s1, s2)

char	s1[ARB], s2[ARB]
int	ip

begin
	do ip = 1, ARB
	    if (s1[ip] != s2[ip])
		return (false)
	    else if (s1[ip] == EOS)
		return (s2[ip] == EOS)
end
