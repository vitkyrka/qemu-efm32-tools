	.syntax unified
	.thumb
	.global _start

	.word 0xdeadbeef	@ Stack
	.word _start + 1	@ PC

_start:
	mov	r0, #0
	mov	r1, #0xde
	ldr	r3, =kernel

	ldr	r2, =dtb
	orr	r3, r3, #1
	bx	r3

	.align 3
	.section ".dtb", "a"
dtb:
	.incbin DTB

	.align 3
	.section ".kernel", "a"
kernel:
	.incbin KERNEL
