AM_CPPFLAGS = -I$(top_srcdir) -lstdc++


bin_PROGRAMS = skintest
skintest_LDADD = $(top_builddir)/glcdskin/libglcdskin.la \
                 $(top_builddir)/glcddrivers/libglcddrivers.la \
                 $(top_builddir)/glcdgraphics/libglcdgraphics.la

skintest_SOURCES = skintest.cpp

