KERNELDIR ?= kernel
IMAGESDIR ?= images

KERNEL ?= ${KERNELDIR}/arch/arm/boot/xipImage
DTB ?= ${KERNELDIR}/arch/arm/boot/dts/efm32gg-dk3750.dtb
ROOTFS ?= ${IMAGESDIR}/root.romfs

.PHONY: clean

all: boot flash

boot: boot.S script.ld ${KERNEL} ${DTB}
	arm-none-eabi-gcc -march=armv7-m -mthumb -nostdlib -o $@ boot.S -T script.ld \
		-DKERNEL=\"${KERNEL}\" -DDTB=\"${DTB}\"

flash: ${ROOTFS}
	dd if=/dev/zero of=$@ bs=16M count=1
	# rootfs starts at 0x8c400000 according to the default bootargs
	dd if=${ROOTFS} of=$@ bs=1M seek=4 conv=notrunc

clean:
	@rm -f boot flash
