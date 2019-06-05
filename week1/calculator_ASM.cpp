#include <cstdio>
#include <cstdlib>

int plus(int x, int y) {
    int retval;
    __asm {
        mov eax, x
        add eax, y
        mov retval, eax
    }
    return retval;
}

int minus(int x, int y) {
    int retval;
    __asm {
        mov eax, x
        sub eax, y
        mov retval, eax
    }
    return retval;
}

int mul(int x, int y) {
    int retval;
    __asm {
        mov eax, x
        mul y
        mov retval, eax
    }
    // overflow to edx, but sizeof(int) == sizeof(eax)
    return retval;
}

int division(int x, int y) {
    int retval;
    __asm {
        mov eax, y
        test eax, eax
        je label1        ; div zero
        mov edx, 0
        mov eax, x
        mov ecx, y
        div ecx
        mov retval, eax
    label1:
    }
    return retval;
}

/* from intel manual:
 * shift value could be imm or CL (ecx low)
 */

int shiftLeft(int x, int y) {
    int retval;
    __asm {
        mov eax, x
        mov ecx, y
        shl eax, cl
        mov retval, eax
    }
    return retval;
}

int shiftRight(int x, int y) {
    int retval;
    __asm {
        mov eax, x
        mov ecx, y
        shr eax, cl
        mov retval, eax
    }
    return retval;
}


int main()
{
    int x = 0;
    int y = 0;
    char op = 0;
    int result = 0;

    int s = 0;
    while (s != 3) {
        printf("NUM OP NUM\n");
#if defined(_WIN32) || defined(WIN32)
    s = scanf_s("%d %c %d", &x, &op, 1, &y);
#else
    s = scanf("%d %c %d", &x, &op, &y);
#endif
    }

    if (op == '+') {
        result = plus(x, y);
    }
    else if (op == '-') {
        result = minus(x, y);
    }
    else if (op == '*') {
        result = mul(x, y);
    }
    else if (op == '/') {
        result = division(x, y);
    }
    else if (op == '<') {
        result = shiftLeft(x, y);
    }
    else if (op == '>') {
        result = shiftRight(x, y);
    }

    printf("%d %c %d = %d\n", x, op, y, result);
    return 0;
}
