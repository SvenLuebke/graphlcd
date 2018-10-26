AM_CXXFLAGS = -I$(top_srcdir) @MY_LIBFREETYPE2_CPPFLAGS@ @libfreetype2_CFLAGS@ @imagemagick_CFLAGS@ @MY_IMAGEMAGICK_CPPFLAGS@

lib_LTLIBRARIES = libglcdgraphics.la
libglcdgraphics_la_SOURCES = bitmap.cpp common.cpp extformats.cpp font.cpp glcd.cpp \
                             image.cpp imagefile.cpp pbm.cpp
                             
libglcdgraphics_la_LDFLAGS = -version-info @GLCDGRAPHICS_MAJOR_VERSION@:@GLCDGRAPHICS_MINOR_VERSION@:@GLCDGRAPHICS_MICRO_VERSION@
libglcdgraphics_la_LIBADD = -ldl @libfreetype2_LIBS@ @imagemagick_LIBS@

pkgincludedir = ${includedir}/glcdgraphics
pkginclude_HEADERS = $(srcdir)/bitmap.h $(srcdir)/font.h $(srcdir)/glcd.h $(srcdir)/image.h \
                     $(srcdir)/imagefile.h $(srcdir)/pbm.h $(srcdir)/extformats.h
                     
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libglcdgraphics.pc
