
SUBDIRS = .

AUTOMAKE_OPTIONS = nostdinc subdir-objects

AM_CFLAGS = -imacros $(top_builddir)/include/config.h -I$(top_srcdir)/include
AM_CCASFLAGS = -imacros $(top_builddir)/include/config.h -I$(top_srcdir)/include

# rpi library
noinst_LIBRARIES = librpi.a
librpi_a_SOURCES = \
  mailbox/mailbox.c \
  mailbox/property.c \
  serial.c \
  start.$(executable_format).S \
  platform.c

# built sources determining things to be built first
BUILT_SOURCES = \
  librpi.a \
  ${top_builddir}/src/arch/${arch_subdir}/${subarch_subdir}/lib$(subarch_subdir).a \
  ${top_builddir}/src/arch/${arch_subdir}/lib$(arch_subdir).a \
  ${top_builddir}/src/loader/libloader.a \
  loader.elf

# no installable programs
noinst_PROGRAMS = loader.elf ${output_img}

# Normal loader building for physical device
loader_elf_SOURCES =
loader_elf_DEPENDENCIES = \
  loader.$(subarch_subdir).$(executable_format).ld \
  librpi.a \
  ${top_builddir}/src/arch/${arch_subdir}/${subarch_subdir}/lib$(subarch_subdir).a \
  ${top_builddir}/src/arch/${arch_subdir}/lib$(arch_subdir).a \
  ${top_builddir}/src/loader/libloader.a
loader_elf_LDFLAGS = \
  -T $(top_srcdir)/src/platform/rpi/loader.$(subarch_subdir).$(executable_format).ld \
  -Wl,-Map,loader.map \
  -Wl,-u,Entry

EXTRA_PROGRAMS = loader.img loader7.img loader8.img
loader_img_DEPENDENCIES = loader.elf
loader_img_SOURCES =
loader.img$(EXEEXT): ${top_builddir}/src/platform/rpi/loader.elf
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --only-keep-debug ${top_builddir}/src/platform/rpi/loader.elf ${top_builddir}/src/platform/rpi/${output_sym}
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --strip-debug ${top_builddir}/src/platform/rpi/loader.elf
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) ${top_builddir}/src/platform/rpi/loader.elf -O binary ${top_builddir}/src/platform/rpi/${output_img}

loader7_img_DEPENDENCIES = loader.elf
loader7_img_SOURCES =
loader7.img$(EXEEXT): ${top_builddir}/src/platform/rpi/loader.elf
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --only-keep-debug ${top_builddir}/src/platform/rpi/loader.elf ${top_builddir}/src/platform/rpi/${output_sym}
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --strip-debug ${top_builddir}/src/platform/rpi/loader.elf
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) ${top_builddir}/src/platform/rpi/loader.elf -O binary ${top_builddir}/src/platform/rpi/${output_img}

loader8_img_DEPENDENCIES = loader.elf
loader8_img_SOURCES =
loader8.img$(EXEEXT): ${top_builddir}/src/platform/rpi/loader.elf
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --only-keep-debug ${top_builddir}/src/platform/rpi/loader.elf ${top_builddir}/src/platform/rpi/${output_sym}
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --strip-debug ${top_builddir}/src/platform/rpi/loader.elf
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) ${top_builddir}/src/platform/rpi/loader.elf -O binary ${top_builddir}/src/platform/rpi/${output_img}
