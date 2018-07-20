lib_LTLIBRARIES = libglcdgraphics.la


AM_CPPFLAGS = -I$(srcdir)/include

libglcdgraphics_la_SOURCES =  \
     font.c glcd.c common.c bitmap.c

AM_CFLAGS = -Wall -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
AM_CXXFLAGS = -Wall

pkgincludedir = ${includedir}/eplayer2
pkginclude_HEADERS = \
           $(srcdir)/include/*.h
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libglcdgraphics.pc