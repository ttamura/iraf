lists

#{ SYSTEM.CL -- Package script task for the SYSTEM package.  This package is
# loaded by the CL upon startup, and is always in the search path.

package	system

task	cmdstr,
	concatenate,
	copy,
	count,
	delete,
	directory,
	files,
	head,
	lprint,
	match,
	mkdir,
	movefiles,
	mtclean,
	$netstatus,
	page,
	pathnames,
	protect,
	rename,
	sort,
	tail,
	tee,
	touch,
	type,
	rewind,
	unprotect,
	help		= "system$x_system.e"

task	mkscript	= "system$mkscript.cl"; hidetask cmdstr
task	$news		= "system$news.cl"

task	allocate	= "hconfig$allocate.cl"
task	gripes		= "hconfig$gripes.cl"
task	deallocate	= "hconfig$deallocate.cl"
task	devstatus	= "hconfig$devstatus.cl"
task	$diskspace	= "hconfig$diskspace.cl"
task	$spy		= "hconfig$spy.cl"

task	$devices	= "system$devices.cl"
task	references	= "system$references.cl"
task	phelp		= "system$phelp.cl"

hidetask mtclean

keep
