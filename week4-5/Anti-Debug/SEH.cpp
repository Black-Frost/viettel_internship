#include <cstdio>
#include <windows.h>

BOOL isDebuggerPresent = TRUE;

EXCEPTION_DISPOSITION ExceptionHandler(
  PEXCEPTION_RECORD ExceptionRecord,
  PVOID EstablisherFrame,
  PCONTEXT ContextRecord,
  PVOID DispatcherContext
) {
  isDebuggerPresent = FALSE;
  ContextRecord->Eip += 1;
  return ExceptionContinueExecution;
}

int main(int argc, char** argv) {
  __asm {
    push ExceptionHandler
    push dword ptr fs:[0]
    mov dword ptr fs:[0], esp

    int 3h

    mov eax, [esp]
    mov dword ptr fs:[0], eax
    add esp, 8
  }
  if (isDebuggerPresent) {
    printf("Stop using debugger\n");
    return 0;
  }
  printf("Find me\n");
  return 0;
}
