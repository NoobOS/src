

.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15HandleInterruptEhj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv

.macro HandleException num
.global _ZN16InterruptManager16HandleException\num\()Ev
_ZN16InterruptManager16HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm


.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleException 0x00
HandleException 0x01


HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:


    # save registers
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs
    
    
    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15HandleInterruptEhj
    # add 55 %esp
    movl %eax, %esp # switch the stack
    
    
    # restore registers
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

_ZN16InterruptManager22IgnoreInterruptRequestEv:  

    iret
    
.data
    interruptnumber: .byte 0
    
    