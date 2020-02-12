.set IRQ_BASE,0x20

.section .text

.extern _ZN16InterruptManager16handlerInterruptEhj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv

.macro HandleExceptionRequest num
.global _ZN16InterruptManager16handlerInterruptRequest\num\()Ev
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager27HandlerInterruptRequest\num\()Ev
_ZN16InterruptManager27HandlerInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F

int_bottom:

    pusha 
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager16handlerInterruptEhj
    add %esp,6
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

    iret
_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret
.data
    interruptnumber: .byte 0



