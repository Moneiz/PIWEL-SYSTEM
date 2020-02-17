.set IRQ_BASE,0x20

.section .text

.extern _ZN11hardwarecom16InterruptManager16handlerInterruptEhj
.global _ZN11hardwarecom16InterruptManager22IgnoreInterruptRequestEv

.macro HandleExceptionRequest num
.global _ZN11hardwarecom16InterruptManager16handlerInterruptRequest\num\()Ev
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN11hardwarecom16InterruptManager27HandlerInterruptRequest\num\()Ev
_ZN11hardwarecom16InterruptManager27HandlerInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0
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

    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    pushl %esp
    push (interruptnumber)
    call _ZN11hardwarecom16InterruptManager16handlerInterruptEhj
    
    mov %eax, %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp

    add $4,%esp

    iret
_ZN11hardwarecom16InterruptManager22IgnoreInterruptRequestEv:
    iret
.data
    interruptnumber: .byte 0



