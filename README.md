Kernel
------

Build a mainline kernel (I used v3.15-rc3) with these two patches applied.
Use `efm32_defconfig`.

- http://lists.infradead.org/pipermail/linux-arm-kernel/2014-May/253002.html
- http://lists.infradead.org/pipermail/linux-arm-kernel/2014-April/249058.html

Root file system
----------------

Clone the Pengutronix BSP and build it using ptxdist according
to the README available in that git:

http://git-public.pengutronix.de/?p=OSELAS.BSP-EnergyMicro-Gecko.git

QEMU
----

Build QEMU from the `efm32` branch of https://github.com/rabinv/qemu/

Bootable images
---------------

In this git, create symlinks to the directory where you built your
kernel and the directory where you built your BSP images.  Then run
make.

    $ ln -s ~/dev/efm-kernel kernel
    $ ln -s ~/dev/OSELAS.BSP-EnergyMicro-Gecko/platform-energymicro-efm32gg-dk3750/images
    $ make

You should now have two images, `boot` and `flash`.

Run
---

    $ qemu-system-arm -nographic -M efm32ggdk3750 -kernel boot -pflash flash
