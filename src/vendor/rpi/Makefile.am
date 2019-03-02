
SUBDIRS = .

AUTOMAKE_OPTIONS = nostdinc subdir-objects

AM_CFLAGS = -imacros $(top_builddir)/config.h -I$(top_srcdir)/include
AM_CCASFLAGS = -imacros $(top_builddir)/config.h -I$(top_srcdir)/include

# rpi library
noinst_LIBRARIES = librpi.a
librpi_a_SOURCES = \
  mailbox/mailbox.c \
  mailbox/property.c \
  parameter.c \
  serial.c \
  start.$(subarch_subdir).$(executable_format).S \
  startup.$(executable_format).S

# built sources determining things to be built first
BUILT_SOURCES = \
  librpi.a \
  ${top_builddir}/src/arch/${arch_subdir}/${subarch_subdir}/lib$(subarch_subdir).a \
  ${top_builddir}/src/arch/${arch_subdir}/lib$(arch_subdir).a \
  ${top_builddir}/src/loader/libloader.a \
  loader.zwerg

# no installable programs
noinst_PROGRAMS = loader.zwerg ${output_img}

# Normal loader building for physical device
loader_zwerg_SOURCES =
loader_zwerg_DEPENDENCIES = \
  loader.$(subarch_subdir).$(executable_format).ld \
  librpi.a \
  ${top_builddir}/src/arch/${arch_subdir}/${subarch_subdir}/lib$(subarch_subdir).a \
  ${top_builddir}/src/arch/${arch_subdir}/lib$(arch_subdir).a \
  ${top_builddir}/src/loader/libloader.a
loader_zwerg_LDFLAGS = \
  -T $(top_srcdir)/src/vendor/rpi/loader.$(subarch_subdir).$(executable_format).ld \
  -Wl,-Map,loader.map \
  -Wl,-u,Entry

EXTRA_PROGRAMS = loader.img loader7.img loader8.img
loader_img_DEPENDENCIES = loader.zwerg
loader_img_SOURCES =
loader.img$(EXEEXT): ${top_builddir}/src/vendor/rpi/loader.zwerg
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --only-keep-debug ${top_builddir}/src/vendor/rpi/loader.zwerg ${top_builddir}/src/vendor/rpi/${output_sym}
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --strip-debug ${top_builddir}/src/vendor/rpi/loader.zwerg
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) ${top_builddir}/src/vendor/rpi/loader.zwerg -O binary ${top_builddir}/src/vendor/rpi/${output_img}

loader7_img_DEPENDENCIES = loader.zwerg
loader7_img_SOURCES =
loader7.img$(EXEEXT): ${top_builddir}/src/vendor/rpi/loader.zwerg
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --only-keep-debug ${top_builddir}/src/vendor/rpi/loader.zwerg ${top_builddir}/src/vendor/rpi/${output_sym}
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --strip-debug ${top_builddir}/src/vendor/rpi/loader.zwerg
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) ${top_builddir}/src/vendor/rpi/loader.zwerg -O binary ${top_builddir}/src/vendor/rpi/${output_img}

loader8_img_DEPENDENCIES = loader.zwerg
loader8_img_SOURCES =
loader8.img$(EXEEXT): ${top_builddir}/src/vendor/rpi/loader.zwerg
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --only-keep-debug ${top_builddir}/src/vendor/rpi/loader.zwerg ${top_builddir}/src/vendor/rpi/${output_sym}
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) --strip-debug ${top_builddir}/src/vendor/rpi/loader.zwerg
	$(BOLTHUR_SERIAL_LOADER_OBJCOPY) ${top_builddir}/src/vendor/rpi/loader.zwerg -O binary ${top_builddir}/src/vendor/rpi/${output_img}
