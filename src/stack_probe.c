/* Self-contained Windows stack probing. Compiler-generated SAFESEH metadata on
 * 32-bit builds avoids disabling /safeseh when linking with hand-written asm.
 */
#if defined(_M_X64) || defined(__x86_64__)
__attribute__((naked)) void pdfutils_stack_probe(void) __asm__("__chkstk");
__attribute__((naked)) void pdfutils_stack_probe(void) {
  __asm__(
    "pushq %r10\n\tpushq %r11\n\t"
    "movq %rax, %r11\n\t"
    "leaq 24(%rsp), %r10\n\t"
    "1:\n\t"
    "cmpq $4096, %r11\n\t"
    "jb 2f\n\t"
    "subq $4096, %r10\n\t"
    "testb $0, (%r10)\n\t"
    "subq $4096, %r11\n\t"
    "jmp 1b\n\t"
    "2:\n\t"
    "subq %r11, %r10\n\t"
    "testb $0, (%r10)\n\t"
    "popq %r11\n\tpopq %r10\n\tretq\n\t");
}
#elif defined(_M_IX86) || defined(__i386__)
__attribute__((naked)) void pdfutils_stack_probe(void) __asm__("__chkstk");
__attribute__((naked)) void pdfutils_stack_probe(void) {
  __asm__(
    "pushl %ecx\n\t"
    "leal 8(%esp), %ecx\n\t"
    "cmpl $4096, %eax\n\t"
    "jb 2f\n\t"
    "1:\n\t"
    "subl $4096, %ecx\n\t"
    "testb $0, (%ecx)\n\t"
    "subl $4096, %eax\n\t"
    "cmpl $4096, %eax\n\t"
    "jae 1b\n\t"
    "2:\n\t"
    "subl %eax, %ecx\n\t"
    "testb $0, (%ecx)\n\t"
    "movl %esp, %eax\n\t"
    "movl %ecx, %esp\n\t"
    "movl (%eax), %ecx\n\t"
    "movl 4(%eax), %eax\n\t"
    "pushl %eax\n\tretl\n\t");
}
#else
#error Unsupported architecture
#endif
