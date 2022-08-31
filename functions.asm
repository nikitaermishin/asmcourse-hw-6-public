section .text

; f1(x) = 0.35x^2 - 0.95x + 2.7
; f2(x) = 1 + 3x
; f3(x) = 1 / (x + 2)
; f4(x) = x^2 - 4
; f5(x) = 4x + 1
; f6(x) = 12
; f7(x) = x^4 + 2x^2

global f1
; f1(x) = 0.35x^2 - 0.95x + 2.7
; RPN x x 35 100 / * * x 95 100 / * - 27 10 / +
f1:
    push ebp
    mov ebp, esp
    sub esp, 8

    fld qword[ebp + 8]
    fld qword[ebp + 8]
    mov dword[ebp - 4], 35
    fild dword[ebp - 4]
    mov dword[ebp - 4], 100
    fild dword[ebp - 4]
    fdivp
    fmulp
    fmulp

    fld qword[ebp + 8]
    mov dword[ebp - 4], 95
    fild dword[ebp - 4]
    mov dword[ebp - 4], 100
    fild dword[ebp - 4]
    fdivp
    fmulp
    fsubp

    mov dword[ebp - 4], 27
    fild dword[ebp - 4]
    mov dword[ebp - 4], 10
    fild dword[ebp - 4]
    fdivp
    faddp

    mov esp, ebp
    pop ebp
    ret

global f2
; f2(x) = 1 + 3x
; RPN 1 3 x * +
f2:
    push ebp
    mov ebp, esp
    sub esp, 8

    fld1
    mov dword[ebp - 4], 3
    fild dword[ebp - 4]
    fld qword[ebp + 8]
    fmulp
    faddp

    mov esp, ebp
    pop ebp
    ret

global f3
; f3(x) = 1 / (x + 2)
; RPN 1 x 2 + /
f3:
    push ebp
    mov ebp, esp
    sub esp, 8

    fld1
    fld qword[ebp + 8]
    mov dword[ebp - 4], 2
    fild dword[ebp - 4]
    faddp
    fdivp

    mov esp, ebp
    pop ebp
    ret

global f4
; f4(x) = x^2 - 4
; RPN -4 x x * +
f4:
    push ebp
    mov ebp, esp
    sub esp, 8

    mov dword[ebp - 4], -4
    fild dword[ebp - 4]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    faddp

    mov esp, ebp
    pop ebp
    ret

global f5
; f5(x) = 4x + 1
; RPN 1 4 x * +
f5:
    push ebp
    mov ebp, esp
    sub esp, 8

    fld1
    mov dword[ebp - 4], 4
    fild dword[ebp - 4]
    fld qword[ebp + 8]
    fmulp
    faddp

    mov esp, ebp
    pop ebp
    ret

global f6
; f6(x) = 12
; RPN 12
f6:
    push ebp
    mov ebp, esp
    sub esp, 8

    mov dword[ebp - 4], 12
    fild dword[ebp - 4]

    mov esp, ebp
    pop ebp
    ret

global f7
; f7(x) = x^4 + 2x^2
; RPN x x x x * * * 2 x x * * +
f7:
    push ebp
    mov ebp, esp
    sub esp, 8

    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fmulp
    fmulp

    mov dword[ebp - 4], 2
    fild dword[ebp - 4]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fmulp

    faddp

    mov esp, ebp
    pop ebp
    ret
