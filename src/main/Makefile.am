xrmcincludedir = ${includedir}/xrmc
xrmcinclude_HEADERS = xrmc.h 

bin_PROGRAMS = xrmc

xrmc_SOURCES = loaddevice.cpp loadparams.cpp xrmc.cpp       xrmc.h         xrmc_main.cpp

xrmc_LDADD = ../libxrmc.la @xraylib_LIBS@
xrmc_CPPFLAGS = -I${srcdir}/../xrmc_math -I${srcdir}/../xrmc_algo -I${srcdir}/../arrayNd -I${srcdir}/../device -I${srcdir}/../source -I${srcdir}/../photon -I${srcdir}/../spectrum -I${srcdir}/../exception -I${srcdir}/../main -I${srcdir}/../gettoken -I${srcdir}/../sample -I${srcdir}/../composition -I${srcdir}/../geom3d -I${srcdir}/../detector -I${srcdir}/../randmt @OPENMP_CXXFLAGS@
xrmc_LDFLAGS = @OPENMP_CXXFLAGS@
if ENABLE_XMIMSIM
xrmc_CPPFLAGS += -I${srcdir}/../xmi-msim @xmi_msim_CFLAGS@ @gmodule_CFLAGS@ \
		-DXRMC_XMIMSIM_LIB=\"$(libdir)/xrmc\"
endif
