.set IRQ_BASE,0x20

.section .text

.extern _ZN11hardwarecom16InterruptManager16handlerInterruptEhj

.macro HandleExceptionRequest num
.global _ZN11hardwarecom16InterruptManager26HandleExceptionRequest\num\()Ev
_ZN11hardwarecom16InterruptManager26HandleExceptionRequest\num\()Ev:
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

HandleExceptionRequest 0x00
HandleExceptionRequest 0x01
HandleExceptionRequest 0x02
HandleExceptionRequest 0x03
HandleExceptionRequest 0x04
HandleExceptionRequest 0x05
HandleExceptionRequest 0x06
HandleExceptionRequest 0x07
HandleExceptionRequest 0x08
HandleExceptionRequest 0x09
HandleExceptionRequest 0x0A
HandleExceptionRequest 0x0B
HandleExceptionRequest 0x0C
HandleExceptionRequest 0x0D
HandleExceptionRequest 0x0E
HandleExceptionRequest 0x0F
HandleExceptionRequest 0x10
HandleExceptionRequest 0x11
HandleExceptionRequest 0x12
HandleExceptionRequest 0x13

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
HandleInterruptRequest 0x31

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


.global _ZN11hardwarecom16InterruptManager22IgnoreInterruptRequestEv
_ZN11hardwarecom16InterruptManager22IgnoreInterruptRequestEv:
    iret
.data
    interruptnumber: .byte 0



