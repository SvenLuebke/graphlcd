AM_CXXFLAGS = -I$(top_srcdir) @LIBFONTCONFIG_CPPFLAGS@
AM_CPPFLAGS =

lib_LTLIBRARIES = libglcdskin.la
libglcdskin_la_SOURCES = cache.cpp config.cpp display.cpp font.cpp function.cpp object.cpp parser.cpp \
                         skin.cpp string_lcd.cpp type.cpp variable.cpp xml.cpp


libglcdskin_la_LDFLAGS = -version-info @GLCDSKIN_MAJOR_VERSION@:@GLCDSKIN_MINOR_VERSION@:@GLCDSKIN_MICRO_VERSION@
libglcdskin_la_LDFLAGS += @LIBFONTCONFIG_LDFLAGS@

pkginclude_HEADERS = cache.h config.h display.h font.h function.h object.h parser.h skin.h string_lcd.h type.h variable.h xml.h
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libglcdskin.pc

