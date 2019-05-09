
SUBDIRS = .

AUTOMAKE_OPTIONS = nostdinc subdir-objects

AM_CFLAGS = -imacros $(top_builddir)/include/config.h -I$(top_srcdir)/include
AM_CCASFLAGS = -imacros $(top_builddir)/include/config.h -I$(top_srcdir)/include

noinst_LIBRARIES = libloader.a
libloader_a_SOURCES = \
  itoa.c \
  main.c \
  memcpy.c \
  printf.c \
  strlen.c \
  strrev.c \
  utoa.c
