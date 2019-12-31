# global data  #
# gcc -o asm_test_1 asm_test_1.s && asm_test_1

        .file   "hello_c.c"
        .def    ___main;        .scl    2;      .type   32;     .endef
        .text
.globl _main
        .def    _main;  .scl    2;      .type   32;     .endef
        .def    _printf;        .scl    2;      .type   32;     .endef
_main:
	# subroutine prologue
        pushl   %ebp
        movl    %esp, %ebp

	# use of Byte instead of Long to test 8 bit 
        movb    $64, %al
	    movb    %al, VAR_A

	pushl	VAR_A
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

	.data
MSG: # msg format
        .asciz "Hello, world %hhd!\12\0" # %hhd for byte (8) and %d for int (32)
VAR_A: # static byte
	.byte 0
