AC_INIT(graphlcd-base,0.0.1,,graphlcd-base)
AM_INIT_AUTOMAKE([foreign dist-xz])
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])

AC_PROG_CC
AC_PROG_CXX
PKG_PROG_PKG_CONFIG

m4_define([libglcddrivers_major_version], [2])
GLCDDRIVERS_MAJOR_VERSION=libglcddrivers_major_version
AC_SUBST(GLCDDRIVERS_MAJOR_VERSION)
m4_define([libglcddrivers_minor_version], [1])
GLCDDRIVERS_MINOR_VERSION=libglcddrivers_minor_version
AC_SUBST(GLCDDRIVERS_MINOR_VERSION)
m4_define([libglcddrivers_micro_version], [0])
GLCDDRIVERS_MICRO_VERSION=libglcddrivers_micro_version
AC_SUBST(GLCDDRIVERS_MICRO_VERSION)

m4_define([libglcdgraphics_major_version], [2])
GLCDGRAPHICS_MAJOR_VERSION=libglcdgraphics_major_version
AC_SUBST(GLCDGRAPHICS_MAJOR_VERSION)
m4_define([libglcdgraphics_minor_version], [1])
GLCDGRAPHICS_MINOR_VERSION=libglcdgraphics_minor_version
AC_SUBST(GLCDGRAPHICS_MINOR_VERSION)
m4_define([libglcdgraphics_micro_version], [0])
GLCDGRAPHICS_MICRO_VERSION=libglcdgraphics_micro_version
AC_SUBST(GLCDGRAPHICS_MICRO_VERSION)

m4_define([libglcdskin_major_version], [2])
GLCDSKIN_MAJOR_VERSION=libglcdskin_major_version
AC_SUBST(GLCDSKIN_MAJOR_VERSION)
m4_define([libglcdskin_minor_version], [1])
GLCDSKIN_MINOR_VERSION=libglcdskin_minor_version
AC_SUBST(GLCDSKIN_MINOR_VERSION)
m4_define([libglcdskin_micro_version], [0])
GLCDSKIN_MICRO_VERSION=libglcdskin_micro_version
AC_SUBST(GLCDSKIN_MICRO_VERSION)

AC_ARG_ENABLE([plugins],
[  --enable-plugins        linker plugins],
[case "${enableval}" in
  yes | "") plugins=yes ;;
  no) plugins=no ;;
  *) plugins=yes ;;
 esac],
[plugins=no])
if test "$plugins" = "yes"; then
  AC_DEFINE(ENABLE_PLUGINS, 1,
	    [Define to enable linker plugins])
fi
AM_CONDITIONAL(PLUGINS, test "$plugins" = "yes") 


AC_ARG_ENABLE([freetype2],
 	AS_HELP_STRING(--disable-freetype2,disable usage of freetype2),
 	[case "${enableval}" in yes | "") am_freetype2=yes ;; no) am_freetype2=no ;; *) am_freetype2=yes ;; esac], [am_freetype2=yes])
  if test "$am_freetype2" = "yes"; then
		PKG_CHECK_MODULES([libfreetype2], [freetype2], [HAVE_FREETYPE2=1])
		AS_IF([test "$HAVE_FREETYPE2" = 1], [AC_SUBST(MY_LIBFREETYPE2_CPPFLAGS,-DHAVE_FREETYPE2)])
	fi

dnl ****** NOT Ready yet...this can be done with pkg-config **********
AC_SUBST(MY_IMAGEMAGICK_CPPFLAGS,[])
AC_SUBST(imagemagick_CFLAGS,[])
AC_SUBST(imagemagick_LIBS,[])
AC_ARG_WITH([imagemagick],
 	AS_HELP_STRING(--with-imagemagick,use imagemagick) )
  if test "x$with_gtk" = "xyes"; then
		PKG_CHECK_MODULES([ImageMagick], [ImageMagick++], [HAVE_IMAGEMAGICK=1])
		AS_IF([test "$HAVE_IMAGEMAGICK" = 1], [AC_SUBST(MY_IMAGEMAGICK_CPPFLAGS,-DHAVE_IMAGEMAGICK)])
	fi

AC_ARG_ENABLE([fontconfig],
 	AS_HELP_STRING(--disable-fontconfig,disable support for fontconfig),
 	[case "${enableval}" in yes | "") am_fontconfig=yes ;; no) am_fontconfig=no ;; *) am_fontconfig=yes ;; esac], [am_fontconfig=yes])
  if test "$am_fontconfig" = "yes"; then
		AC_CHECK_LIB([fontconfig], [FcNameParse])
		AS_IF([test "$ac_cv_lib_fontconfig_FcNameParse" = yes], [AC_SUBST(LIBFONTCONFIG_CPPFLAGS,-DHAVE_FONTCONFIG) AC_SUBST(LIBFONTCONFIG_LDFLAGS,-lfontconfig)])
	fi

AC_ARG_ENABLE([debug],
 	AS_HELP_STRING(--disable-debug,disable debug output),
 	[case "${enableval}" in yes | "") am_debug=yes ;; no) am_debug=no ;; *) am_debug=yes ;; esac], [am_debug=yes])
  if test "$am_debug" = "yes"; then
    AC_DEFINE(HAVE_DEBUG, 1, [Define to enable debug output])
		AC_SUBST(MY_DEBUG_CPPFLAGS,-DHAVE_DEBUG)
	fi

AC_ARG_ENABLE([driver_vncserver],
 	AS_HELP_STRING(--disable-driver-vncserver,disable usage of vncserver),
 	[case "${enableval}" in yes | "") am_driver_vncserver=yes ;; no) am_driver_vncserver=no ;; *) am_driver_vncserver=yes ;; esac], [am_driver_vncserver=yes])
  if test "$am_driver_vncserver" = "yes"; then
    PKG_CHECK_MODULES([libvncserver], [libvncserver], [HAVE_LIBVNCSERVER=1])
    AS_IF([test "$HAVE_LIBVNCSERVER" = 1], [AM_CONDITIONAL([HAVE_DRIVER_VNCSERVER], test "$am_driver_vncserver" = "yes")
                                            AC_SUBST(MY_VNCSERVER_CPPFLAGS,-DHAVE_VNCSERVER)])  
  fi
	
AC_ARG_ENABLE([driver-ax206dpf],
 	AS_HELP_STRING(--disable-driver-ax206dpf,disable driver ax206dpf),
 	[case "${enableval}" in yes | "") am_driver_ax206dpf=yes ;; no) am_driver_ax206dpf=no ;; *) am_driver_ax206dpf=no ;; esac], [am_driver_ax206dpf=no])
  AM_CONDITIONAL([HAVE_DRIVER_AX206DPF], test "$am_driver_ax206dpf" = "yes")

AC_ARG_ENABLE([driver-picolcd-256x64],
 	AS_HELP_STRING(--disable-driver-picolcd-256x64,disable driver picolcd-256x64),
 	[case "${enableval}" in yes | "") am_driver_picolcd_256x64=yes ;; no) am_driver_picolcd_256x64=no ;; *) am_driver_picolcd_256x64=no ;; esac], [am_driver_picolcd_256x64=no])
  AM_CONDITIONAL([HAVE_DRIVER_PICOLCD_256X64], test "$am_driver_picolcd_256x64" = "yes")

AC_ARG_ENABLE([driver-ili9341],
 	AS_HELP_STRING(--disable-driver-ili9341,disable driver ili9341),
 	[case "${enableval}" in yes | "") am_driver_ili9341=yes ;; no) am_driver_ili9341=no ;; *) am_driver_ili9341=no ;; esac], [am_driver_ili9341=no])
  AM_CONDITIONAL([HAVE_DRIVER_ILI9341], test "$am_driver_ili9341" = "yes")

AC_ARG_ENABLE([driver-ssd1306],
 	AS_HELP_STRING(--disable-driver-ssd1306,disable driver ssd1306),
 	[case "${enableval}" in yes | "") am_driver_ssd1306=yes ;; no) am_driver_ssd1306=no ;; *) am_driver_ssd1306=no ;; esac], [am_driver_ssd1306=no])
  AM_CONDITIONAL([HAVE_DRIVER_SSD1306], test "$am_driver_ssd1306" = "yes")

AC_ARG_ENABLE([driver-ssd1322ftdi],
 	AS_HELP_STRING(--disable-driver-ssd1322ftdi,disable driver ssd1322ftdi),
 	[case "${enableval}" in yes | "") am_driver_ssd1322ftdi=yes ;; no) am_driver_ssd1322ftdi=no ;; *) am_driver_ssd1322ftdi=yes ;; esac], [am_driver_ssd1322ftdi=yes])
  if test "$am_driver_ssd1322ftdi" = "yes"; then
    PKG_CHECK_MODULES([libftdi], [libftdi], [HAVE_LIBFTDI=1])
    AM_CONDITIONAL([HAVE_DRIVER_SSD1322FTDI], test "$am_driver_ssd1322ftdi" = "yes")
  fi

CXXFLAGS="$CXXFLAGS -Wall -Woverloaded-virtual -fPIC -D_GNU_SOURCE"
dnl CXXFLAGS="$CXXFLAGS -MMD"

AC_CONFIG_HEADERS([config.h:config.hin])

AC_CONFIG_FILES([
Makefile \
glcdgraphics/Makefile \
glcdgraphics/libglcdgraphics.pc \
glcddrivers/Makefile \
glcddrivers/libglcddrivers.pc \
glcdskin/Makefile \
glcdskin/libglcdskin.pc \
tools/Makefile \
tools/convpic/Makefile \
tools/crtfont/Makefile \
tools/genfont/Makefile \
tools/lcdtestpattern/Makefile \
tools/showpic/Makefile \
tools/showtext/Makefile \
tools/skintest/Makefile \
])
AC_OUTPUT

