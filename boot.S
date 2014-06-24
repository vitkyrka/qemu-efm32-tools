	.syntax unified
	.thumb
	.global _start

	.word 0xdeadbeef	@ Stack
	.word _start + 1	@ PC

_start:
	ldr	r0, =#0xfff20000
	mov	r1, #65
	str	r1, [r0]

	# Make the second CPU loop here
	ldr	r0, =#0xE00FFFE0
	ldr	r0, [r0]
	teq	r0, #0
	bne	.

	ldr	r0, =#0xfff20000
	mov	r1, #66
	str	r1, [r0]

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
