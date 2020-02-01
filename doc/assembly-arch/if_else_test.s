# global data  #
# gcc -o asm_test_1 asm_test_1.s && asm_test_1

        .file   "hello_c.c"
        .def    ___main;        .scl    2;      .type   32;     .endef
        .text
.globl _main
        .def    _main;  .scl    2;      .type   32;     .endef
_main:
	# subroutine prologue
        pushl   %ebp
        movl    %esp, %ebp

	movl 	$-2, %eax	# eax = 2
	#movl 	$1, %ebx	# ebx = 1
	#movl 	$0, %ecx	# ecx = 0

	cmpl 	$0, %eax 	# is eax > 0
	#cmovl 	%ecx, %eax 	# false
	#cmovge	%ebx, %eax	# true
	jge 	.true
	movl 	$0, %eax	# false
	jmp .done


.true:
	movl 	$1, %eax 	# true
	jmp .done

.done:
	pushl	%eax	
	pushl 	$MSG
        call    _printf

	# exit
        movl    $0, %eax
        leave
	ret

	.data
MSG: # msg format
        .asciz "Hello, world %hhd!\12\0" # %hhd for byte (8) and %d for int (32)
