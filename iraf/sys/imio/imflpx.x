# Copyright(c) 1986 Association of Universities for Research in Astronomy Inc.


# IMFLPS -- Flip an array of SHORT sized elements.

procedure imflps (a, npix)

short	a[npix], temp
int	npix, i, right

begin
	right = npix + 1

	do i = 1, (npix + 1) / 2 {
	    temp = a[right-i]
	    a[right-i] = a[i]
	    a[i] = temp
	}
end


# IMFLPL -- Flip an array of LONG sized elements.

procedure imflpl (a, npix)

long	a[npix], temp
int	npix, i, right

begin
	right = npix + 1

	do i = 1, (npix + 1) / 2 {
	    temp = a[right-i]
	    a[right-i] = a[i]
	    a[i] = temp
	}
end
