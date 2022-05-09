.section .text
.code16
.globl _start
_start:

movw $0x7c0, %ax
addw $0x20, %ax
movw %ax, %ss
movw $0x1000, %sp

movw $0x7c0, %ax
movw %ax, %ds

.set OFFSET, my_msg - _start
movw $OFFSET, %si

movb $0xe, %ah
mov $0, %dx

print_loop:
	lodsb
	cmp $0, %al
	je done
	int $0x10
	jmp print_loop

done:
	hlt
	jmp done

my_msg:
	.asciz "Hello from the bootloader!\n"

remainder:
	.set MBR_REMAINDER, 510 - (remainder - _start)
	.fill MBR_REMAINDER, 1, 0x0
	.word 0xaa55
