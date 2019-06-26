#include <cstdio>
#include <windows.h>


LONG CALLBACK ExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) {
  PCONTEXT ctx = ExceptionInfo->ContextRecord;
  if (ctx->Dr0 != 0 || ctx->Dr1 != 0 || ctx->Dr2 != 0 || ctx->Dr3 != 0) {
    printf("Stop using debugger\n");
    exit(0);
  }
  ctx->Eip += 2;
  return EXCEPTION_CONTINUE_EXECUTION;
}

int main(int argc, char** argv) {
  AddVectoredExceptionHandler(0, ExceptionHandler);
  __asm int 1h;
  printf("Find me\n");
  return 0;
}
