section .text
    GLOBAL _io_hlt, _io_cli, _io_sti, _io_stihlt
    GLOBAL _io_in8, _io_in16, _io_in32
    GLOBAL _io_out8, _io_out16, _io_out32
    GLOBAL _io_load_eflags, _io_store_eflags
    GLOBAL _load_gdtr, _load_idtr
    GLOBAL _load_cr0, _store_cr0
    GLOBAL _load_tr, _farjmp, _taskswitch3, _taskswitch4
    GLOBAL _asm_inthandler20, _asm_inthandler21, _asm_inthandler27, _asm_inthandler2c
    EXTERN inthandler20, inthandler21, inthandler27, inthandler2c

_io_hlt:
    HLT
    RET

_io_cli:
    CLI
    RET

_io_sti:
    STI
    RET

_io_stihlt:
    STI
    HLT
    RET

_io_in8:
    MOV EDX,[ESP+4]
    MOV EAX,0
    IN AL,DX
    RET

_io_in16:
    MOV EDX,[ESP+4]
    MOV EAX,0
    IN AX,DX
    RET

_io_in32:
    MOV EDX,[ESP+4]
    IN EAX,DX
    RET

_io_out8:
    MOV EDX,[ESP+4]
    MOV AL,[ESP+8]
    OUT DX,AL
    RET

_io_out16:
    MOV EDX,[ESP+4]
    MOV EAX,[ESP+8]
    OUT DX,AX
    RET

_io_out32:
    MOV EDX,[ESP+4]
    MOV EAX,[ESP+8]
    OUT DX,EAX
    RET

_io_load_eflags:
    PUSHFD
    POP EAX
    RET

_io_store_eflags:
    MOV EAX,[ESP+4]
    PUSH EAX
    POPFD
    RET

_load_gdtr:
    MOV AX,[ESP+4]
    MOV [ESP+6],AX
    LGDT [ESP+6]
    RET

_load_idtr:
    MOV AX,[ESP+4]
    MOV [ESP+6],AX
    LIDT [ESP+6]
    RET

_load_cr0:
    MOV EAX,CR0
    RET

_store_cr0:
    MOV EAX,[ESP+4]
    MOV CR0,EAX
    RET

_load_tr:
    LTR [ESP+4]
    RET

_farjmp:
    JMP FAR [ESP+4]
    RET

_taskswitch3:
    JMP 3*8:0
    RET

_taskswitch4:
    JMP 4*8:0
    RET

_asm_inthandler20:
    PUSH ES
    PUSH DS
    PUSHAD
    MOV EAX,ESP
    PUSH EAX
    MOV AX,SS
    MOV DS,AX
    MOV ES,AX
    CALL inthandler20
    POP EAX
    POPAD
    POP DS
    POP ES
    IRETD

_asm_inthandler21:
    PUSH ES
    PUSH DS
    PUSHAD
    MOV EAX,ESP
    PUSH EAX
    MOV AX,SS
    MOV DS,AX
    MOV ES,AX
    CALL inthandler21
    POP EAX
    POPAD
    POP DS
    POP ES
    IRETD

_asm_inthandler27:
    PUSH ES
    PUSH DS
    PUSHAD
    MOV EAX,ESP
    PUSH EAX
    MOV AX,SS
    MOV DS,AX
    MOV ES,AX
    CALL inthandler27
    POP EAX
    POPAD
    POP DS
    POP ES
    IRETD

_asm_inthandler2c:
    PUSH ES
    PUSH DS
    PUSHAD
    MOV EAX,ESP
    PUSH EAX
    MOV AX,SS
    MOV DS,AX
    MOV ES,AX
    CALL inthandler2c
    POP EAX
    POPAD
    POP DS
    POP ES
    IRETD
    