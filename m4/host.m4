
AC_DEFUN([BOLTHUR_SERIAL_LOADER_SET_HOST], [
  AH_TEMPLATE([ELF32], [Define to 1 for 32 bit ELF targets])
  AH_TEMPLATE([ELF64], [Define to 1 for 64 bit ELF targets])

  case "${host_cpu}" in
  arm)
    arch_subdir=arm
    host_bfd=elf32-littlearm
    output_img=loader.img
    output_sym=loader.sym
    executable_format=32
    AC_DEFINE([ELF32], [1])

    case "${DEVICE}" in
    rpi2_b_rev1)
      CFLAGS="${CFLAGS} -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv3 -mfloat-abi=hard"
      subarch_subdir=v7
      vendor_subdir=rpi
      output_img=loader7.img
      output_sym=loader7.sym
      AC_DEFINE([ELF32])
      AC_DEFINE([BCM2709], [1], [Define to 1 for BCM2709 chip])
      ;;
    rpi_zero_w)
      CFLAGS="${CFLAGS} -march=armv6zk -mtune=arm1176jzf-s -mfpu=vfpv2 -mfloat-abi=hard"
      subarch_subdir=v6
      vendor_subdir=rpi
      AC_DEFINE([BCM2708], [1], [Define to 1 for BCM2708 chip])
      ;;
    rpi3_b)
      CFLAGS="${CFLAGS} -march=armv8-a -mtune=cortex-a53 -mfpu=neon-vfpv4 -mfloat-abi=hard"
      subarch_subdir=v8
      vendor_subdir=rpi
      output_img=loader8.img
      output_sym=loader8.sym
      AC_DEFINE([BCM2710], [1], [Define to 1 for BCM2710 chip])
      ;;
    *)
      AC_MSG_ERROR([unsupported host vendor])
      ;;
    esac
    ;;
  aarch64)
    arch_subdir=arm
    host_bfd=elf64-littleaarch64
    executable_format=32
    AC_DEFINE([ELF64], [1])

    case "${DEVICE}" in
    rpi3_b)
      CFLAGS="${CFLAGS} -march=armv8-a -mtune=cortex-a53"
      subarch_subdir=v8
      vendor_subdir=rpi
      output_img=loader8.img
      output_sym=loader8.sym
      AC_DEFINE([BCM2710], [1])
      ;;
    *)
      AC_MSG_ERROR([unsupported host vendor])
      ;;
    esac
    ;;
  *)
    AC_MSG_ERROR([unsupported host CPU])
    ;;
  esac

  copy_flags="-I ${host_bfd} -O ${host_bfd}"

  AC_DEFINE_UNQUOTED([ARCH], [${arch_subdir}], [bolthur/serial-loader target architecture])
  AC_DEFINE_UNQUOTED([SUBARCH], [${subarch_subdir}], [bolthur/serial-loader target subarchitecture])
  AC_DEFINE_UNQUOTED([VENDOR], [${vendor_subdir}], [bolthur/serial-loader target vendor])
  AC_SUBST(arch_subdir)
  AC_SUBST(subarch_subdir)
  AC_SUBST(vendor_subdir)
  AC_SUBST(output_img)
  AC_SUBST(output_sym)
  AC_SUBST(host_bfd)
  AC_SUBST(copy_flags)
  AC_SUBST(executable_format)
])

AC_DEFUN([BOLTHUR_SERIAL_LOADER_SET_FLAGS], [
  CFLAGS="${CFLAGS} -ffreestanding -fno-exceptions -Wall -Wextra -Werror -Wpedantic -Wconversion -Wpacked -Wpacked-bitfield-compat -Wpacked-not-aligned -nodefaultlibs -std=c18"
  LDFLAGS="${LDFLAGS} -nostdlib"
])

AC_DEFUN([BOLTHUR_SERIAL_LOADER_PROG_OBJCOPY], [
  AC_CHECK_TOOL([BOLTHUR_SERIAL_LOADER_OBJCOPY], [objcopy])
  AC_CACHE_CHECK([whether objcopy generates $host_bfd],
    [ac_cv_objcopy_supports_host_bfd],
    [if test "$BOLTHUR_SERIAL_LOADER_OBJCOPY" --info 2>&1 < /dev/null | grep "$host_bfd" > /dev/null; then
      ac_cv_objcopy_supports_host_bfd=no
    else
      ac_cv_objcopy_supports_host_bfd=yes
    fi]
  )
  if test ac_cv_objcopy_supports_host_bfd = no; then
    AC_MSG_ERROR([unsupported host BFD])
  fi
])
