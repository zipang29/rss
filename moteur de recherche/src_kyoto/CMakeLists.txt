project(kyoto)

add_library(kyoto STATIC
	cmdcommon.h
	kccachedb.cc
	kccachedb.h
	kccacheex.cc
	kccommon.h
	kccompare.h
	kccompress.h
	kcdb.h
	kcdbext.h
	kcdirdb.h
	kcfile.h
	kchashdb.cc
	kchashdb.h
	kchashex.cc
	kchashmgr.cc
	kclangc.h
	kcmap.h
	kcplantdb.h
	kcpolydb.h
	kcprotodb.h
	kcregex.h
	kcstashdb.cc
	kcstashdb.h
	kctextdb.h
	kcthread.cc
	kcthread.h
	kcutil.h
	myconf.h

	kccachedb.cc
	kccachedb.h
	kccacheex.cc
	kccommon.h
	kccompare.cc
	kccompare.h
	kccompress.cc
	kccompress.h
	kcdb.cc
	kcdbext.cc
	kcdictmgr.cc
	kcdirdb.cc
	kcdirex.cc
	kcdirmgr.cc
	kcdirtest.cc
	kcfile.cc
	kcforestex.cc
	kcforestmgr.cc
	kcgrassex.cc
	kcgrasstest.cc
	kchashdb.cc
	kchashex.cc
	kchashmgr.cc
	kclangc.cc
	kcmap.cc
	kcmrex.cc
	kcplantdb.cc
	kcpolydb.cc
	kcpolyex.cc
	kcpolymgr.cc
	kcpolytest.cc
	kcprotodb.cc
	kcprotoex.cc
	kcregex.cc
	kcstashdb.cc
	kctextdb.cc
	kcthread.cc
	kctreeex.cc
	kctreemgr.cc
	kcutil.cc
	kcutilmgr.cc
	kcvisex.cc
)

if (CMAKE_COMPILER_IS_GNUCC)
	set_property( TARGET main APPEND_STRING PROPERTY COMPILE_FLAGS -W)
elseif (MSVC)
	set_property(TARGET kyoto APPEND_STRING PROPERTY COMPILE_FLAGS /W0)
endif (CMAKE_COMPILER_IS_GNUCC)