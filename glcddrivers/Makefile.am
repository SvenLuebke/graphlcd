AM_CXXFLAGS = -I$(top_srcdir) @MY_DEBUG_CPPFLAGS@

lib_LTLIBRARIES = libglcddrivers.la
libglcddrivers_la_SOURCES = common.cpp config.cpp driver.cpp drivers.cpp port.cpp simlcd.cpp framebuffer.cpp \
                            gu140x32f.cpp gu256x64-372.cpp gu256x64-3900.cpp hd61830.cpp ks0108.cpp image.cpp sed1330.cpp sed1520.cpp \
                            t6963c.cpp noritake800.cpp serdisp.cpp avrctl.cpp g15daemon.cpp network.cpp gu126x64D-K610A4.cpp \
                            dm140gink.cpp usbserlcd.cpp st7565r-reel.cpp

if HAVE_DRIVER_AX206DPF
libglcddrivers_la_SOURCES += ax206dpf.cpp
endif
if HAVE_DRIVER_PICOLCD_256X64
libglcddrivers_la_SOURCES += picoLCD_256x64.cpp
endif
if HAVE_DRIVER_ILI9341
libglcddrivers_la_SOURCES += ili9341.cpp
endif
if HAVE_DRIVER_SSD1306
libglcddrivers_la_SOURCES += ssd1306.cpp
endif
if HAVE_DRIVER_SSD1322FTDI
libglcddrivers_la_SOURCES += iodrv_libftdi.cpp ssd1322ftdi.cpp
AM_CXXFLAGS += @libftdi1_CFLAGS@
endif
if HAVE_DRIVER_VNCSERVER
libglcddrivers_la_SOURCES += vncserver.cpp
AM_CXXFLAGS += @MY_VNCSERVER_CPPFLAGS@ @libvncserver_CFLAGS@
endif

libglcddrivers_la_LDFLAGS = -version-info @GLCDDRIVERS_MAJOR_VERSION@:@GLCDDRIVERS_MINOR_VERSION@:@GLCDDRIVERS_MICRO_VERSION@ \
                            -lpthread @libftdi1_LIBS@ @libvncserver_LIBS@ @libjpeg_LIBS@ @libturbojpeg_LIBS@ @zlib_LIBS@ \
                            @libsystemd_LIBS@

pkgincludedir = ${includedir}/glcddrivers
pkginclude_HEADERS = $(srcdir)/config.h $(srcdir)/driver.h $(srcdir)/drivers.h
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libglcddrivers.pc
