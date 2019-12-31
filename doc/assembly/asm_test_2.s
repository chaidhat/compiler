# global data  #
# gcc -o asm_test_1 asm_test_1.s && asm_test_1

        .file   "hello_c.c"
        .def    ___main;        .scl    2;      .type   32;     .endef
        .text
.globl _main
        .def    _main;  .scl    2;      .type   32;     .endef
_main:
	# exit
        movl    $0, %eax
        leave
	ret

