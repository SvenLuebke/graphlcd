AM_CXXFLAGS = -I$(top_srcdir) @LIBFONTCONFIG_CPPFLAGS@

lib_LTLIBRARIES = libglcdskin.la
libglcdskin_la_SOURCES = cache.cpp config.cpp display.cpp font.cpp function.cpp object.cpp parser.cpp \
                         skin.cpp string_lcd.cpp type.cpp variable.cpp xml.cpp


libglcdskin_la_LDFLAGS = -version-info @GLCDSKIN_MAJOR_VERSION@:@GLCDSKIN_MINOR_VERSION@:@GLCDSKIN_MICRO_VERSION@
libglcdskin_la_LDFLAGS += @LIBFONTCONFIG_LDFLAGS@

pkgincludedir = ${includedir}/glcdskin
pkginclude_HEADERS = $(srcdir)/cache.h $(srcdir)/config.h $(srcdir)/display.h $(srcdir)/font.h \
                     $(srcdir)/function.h $(srcdir)/object.h $(srcdir)/parser.h $(srcdir)/skin.h \
                     $(srcdir)/string_lcd.h $(srcdir)/type.h $(srcdir)/variable.h $(srcdir)/xml.h
                     
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libglcdskin.pc

