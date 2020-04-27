
AC_DEFUN([BOLTHUR_SERIAL_LOADER_SET_FLAG], [
  # warnings
  CFLAGS="${CFLAGS} -Wall -Wextra -Werror -Wpedantic -Wconversion -Wpacked"
  CFLAGS="${CFLAGS} -Wpacked-bitfield-compat -Wpacked-not-aligned"
  # generic
  CFLAGS="${CFLAGS} -ffreestanding -fno-exceptions -nodefaultlibs -std=c18"

  # debug parameter
  AS_IF([test "x$with_debug" == "xyes"], [
    CFLAGS="${CFLAGS} -g"
  ])
  # optimization level
  case "${with_opt}" in
    no | 0)
      CFLAGS="${CFLAGS} -O0"
      ;;
    1)
      CFLAGS="${CFLAGS} -O1"
      ;;
    2)
      CFLAGS="${CFLAGS} -O2"
      ;;
    3)
      CFLAGS="${CFLAGS} -O3"
      ;;
    s)
      CFLAGS="${CFLAGS} -Os"
      ;;
    g)
      CFLAGS="${CFLAGS} -Og"
      ;;
    *)
      CFLAGS="${CFLAGS} -O2"
      ;;
  esac

  LDFLAGS="${LDFLAGS} -nostdlib"
])
