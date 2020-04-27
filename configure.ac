
AC_PREREQ(2.57)
AC_INIT([bolthur-serial-loader], [0.1], [https://github.com/bolthur/serial-loader/issues], [bolthur-serial-loader], [https://github.com/bolthur/serial-loader])
AC_COPYRIGHT([Copyright (C) 2019 bolthur project])

# Add hosted toolchain to path
AC_SUBST(PATH, "/opt/bolthur/cross/bin:${PATH}")

AC_CONFIG_AUX_DIR([build-aux/config])
AC_CONFIG_MACRO_DIR([build-aux/m4])

AC_CANONICAL_HOST
AC_CONFIG_SRCDIR([src/loader/main.c])
AM_INIT_AUTOMAKE()
AC_CONFIG_HEADERS([include/config.h])

AC_LANG([C])

AC_ARG_ENABLE([device],
  AC_HELP_STRING(
    [--enable-device],
    [set target device to compile for [possible targets: rpi2_b_rev1]]
  ),
  [enable_device=$enableval],
  [AC_MSG_ERROR([Target device missing])]
)

AC_ARG_WITH(
  [debug],
  AC_HELP_STRING(
    [--with-debug],
    [compile with debug symbols]
  ),
  [with_debug=yes]
)

AC_ARG_WITH(
  [opt],
  AC_HELP_STRING(
    [--with-opt],
    [compile with specific code optimization level [possible values: 0, 1, 2, 3, s, g | default: 2]]
  ),
  [with_opt=$withval]
)

# set target and flags
BOLTHUR_SERIAL_LOADER_SET_DEVICE
BOLTHUR_SERIAL_LOADER_SET_HOST

# set normal flags
BOLTHUR_SERIAL_LOADER_SET_FLAG

# conftest
AC_LANG_CONFTEST([AC_LANG_SOURCE([[const char hw[] = "Hello, World\r\n";]])])

# checks for programs
AC_PROG_CC
AM_PROG_AS
AC_PROG_RANLIB
AM_PROG_AR
AC_PROG_CXX
BOLTHUR_SERIAL_LOADER_PROG_OBJCOPY

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE
AC_C_RESTRICT

# Some compiler dependent features.
AC_DEFINE_UNQUOTED([PACKED], [__attribute__((__packed__))], [Keyword for packing structures.])
AC_DEFINE_UNQUOTED([SECTION(x)], [__attribute__((__section__(x)))], [Keyword for section placement.])
AC_DEFINE_UNQUOTED([ALIGNED(x)], [__attribute__((__aligned__(x)))], [Keyword for alignment.])
AC_DEFINE_UNQUOTED([PACKED_ALIGNED(x)], [__attribute__((__packed__, __aligned__(x)))], [Keyword for packing and aligning structures.])
AC_DEFINE_UNQUOTED([OPT_NONE], [__attribute__((__optimize__( "O0" )))], [Disable optimization])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  src/loader/Makefile
  src/arch/Makefile
  src/arch/arm/Makefile
  src/arch/arm/v6/Makefile
  src/arch/arm/v7/Makefile
  src/arch/arm/v8/Makefile
  src/platform/Makefile
  src/platform/rpi/Makefile
])

AC_OUTPUT
