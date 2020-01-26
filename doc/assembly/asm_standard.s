# global data  #
# gcc -o asm_standard asm_standard.s && asm_standard

.file   "asm_standard.c"
.data
MSG: # msg format
        .asciz "Hello, world %hhd!\12\0" # %hhd for byte (8) and %d for int (32)
VAR_A: # static byte
        .byte 0
VAR_B: # static long
        .long 0
.text
.globl _main
_main:
	# subroutine prologue
    pushl   %ebp
    movl    %esp, %ebp
    
    # declare var at -4 ebp
    pushl   $8

    # declare var at -8 ebp
    pushl   $9

	# use of Byte instead of Long to test 8 bit 
    movb    $64, %al
    movb    %al, VAR_A

	pushl	VAR_A
	call print_int

    # use prev declared var at -4 ebp
    movl    -4(%ebp), %ebx
    movl    %ebx, VAR_B

	pushl	VAR_B
	call print_int

	# exit
    movl    $0, %eax
    leave
	ret

print_int:
	# subroutine prologue
	pushl	%ebp
	movl 	%esp, %ebp

	pushl	8(%ebp)	# parameters are 8 bytes (esp and ebp are in the middle)
	pushl 	$MSG
        call    _printf

	# subroutine epilogue
	movl 	%ebp, %esp
	popl 	%ebp
	ret 	#pop %eax; jmp %eax

