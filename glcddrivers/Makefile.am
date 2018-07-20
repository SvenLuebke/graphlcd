lib_LTLIBRARIES = libglcddrivers.la


AM_CPPFLAGS = -I$(srcdir)/include

libglcddrivers_la_SOURCES =  \
     port.c \
     noritake800.c picoLCD_256x64.c sed1330.c \
     usbserlcd.c

AM_CFLAGS = -Wall -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
AM_CXXFLAGS = -Wall

pkgincludedir = ${includedir}/eplayer2
pkginclude_HEADERS = \
           $(srcdir)/include/*.h
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libglcdgraphics.pc