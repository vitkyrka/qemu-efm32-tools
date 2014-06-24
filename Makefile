KERNELDIR ?= kernel
IMAGESDIR ?= images

VMLINUX ?= ${KERNELDIR}/vmlinux
KERNEL ?= ${KERNELDIR}/arch/arm/boot/Image
DTB ?= ${KERNELDIR}/arch/arm/boot/dts/ducati.dtb
ROOTFS ?= ${IMAGESDIR}/root.romfs

CC=arm-none-eabi-gcc -fno-short-enums
CFLAGS=-march=armv7-m -mthumb
LDFLAGS=-nostdlib -Tscript.ld

.PHONY: clean

all: boot flash

boot.o: boot.S ${KERNEL} ${DTB}
	arm-none-eabi-gcc -c -march=armv7-m -mthumb -o $@ $< \
		-DKERNEL=\"${KERNEL}\" -DDTB=\"${DTB}\"

rproc.o: rproc.c ${VMLINUX}
rproc.o: CFLAGS+=-D__LOG_BUF_LEN=0x$(shell nm --format posix ${VMLINUX} | grep -i __log_buf | cut -d" " -f 4)

boot: boot.o rproc.o ${VMLINUX} script.ld
	arm-none-eabi-gcc -o $@ -nostdlib -Tscript.ld boot.o rproc.o \
		-Wl,--defsym,__log_buf=0x$(shell nm --format posix ${VMLINUX} | grep -i __log_buf | cut -d' ' -f 3)

# boot: boot.S rproc.c script.ld ${KERNEL} ${DTB}
# 	arm-none-eabi-gcc -march=armv7-m -mthumb -nostdlib -o $@ rproc.c boot.S -T script.ld \
# 		-DKERNEL=\"${KERNEL}\" -DDTB=\"${DTB}\"

flash: ${ROOTFS}
	dd if=/dev/zero of=$@ bs=16M count=1
	# rootfs starts at 0x8c400000 according to the default bootargs
	dd if=${ROOTFS} of=$@ bs=1M seek=4 conv=notrunc

clean:
	@rm -f boot *.o flash
