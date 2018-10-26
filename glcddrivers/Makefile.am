lib_LTLIBRARIES = libglcddrivers.la
libglcddrivers_la_SOURCES = common.cpp config.cpp driver.cpp drivers.cpp port.cpp simlcd.cpp framebuffer.cpp \
                            gu140x32f.cpp gu256x64-372.cpp gu256x64-3900.cpp hd61830.cpp ks0108.cpp image.cpp sed1330.cpp sed1520.cpp \
                            t6963c.cpp noritake800.cpp serdisp.cpp avrctl.cpp g15daemon.cpp network.cpp gu126x64D-K610A4.cpp \
                            dm140gink.cpp usbserlcd.cpp st7565r-reel.cpp
libglcddrivers_la_LDFLAGS = -version-info @GLCDDRIVERS_MAJOR_VERSION@:@GLCDDRIVERS_MINOR_VERSION@:@GLCDDRIVERS_MICRO_VERSION@  -lpthread


pkgincludedir = ${includedir}/glcddrivers
pkginclude_HEADERS = $(srcdir)/config.h $(srcdir)/driver.h $(srcdir)/drivers.h
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libglcddrivers.pc
