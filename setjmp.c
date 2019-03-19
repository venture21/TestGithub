#include <stdio.h>
#include <setjmp.h>

// 실행시  "second"가 출력된 뒤
// "main"을 출력하고 프로그램이 종료 된다. 
// setjmp()는  현재 상태를 스택에 저장한 뒤
// longjmp()호출시 스택에 저장된 값을 꺼내서
// if(!setjmp(buf))위치로 이동한다. 
 
static jmp_buf buf;

void second(void) {
    printf("second\n");         // prints
    longjmp(buf,1);             // jumps back to where setjmp was called - making setjmp now return 1
}

void first(void) {
    second();
    printf("first\n");          // does not print
}

int main() {   
    if (!setjmp(buf))
        first();                // when executed, setjmp returned 0
    else                        // when longjmp jumps back, setjmp returns 1
        printf("main\n");       // prints

    return 0;
}
