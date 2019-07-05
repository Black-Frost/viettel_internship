#include <cstdio>
#include <windows.h>

int main(int argc, char** argv) {
  HANDLE process = GetCurrentProcess();
  DWORD isDebuggerPresent = 0;

  // NTSTATUS s = NtQueryInformationProcess(process, ProcessDebugPort, &isDebuggerPresent, sizeof(DWORD), NULL);
  PVOID NtQueryInformationProcess = (PVOID)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
  if (NtQueryInformationProcess == NULL) {
    printf("Failed\n");
    return 1;
  }
  __asm {
    push ecx
    push eax

    mov ecx, NtQueryInformationProcess
    lea eax, [isDebuggerPresent]
    push 0                ; NULL
    push 4                ; DWORD size
    push eax
    push 7                ; DEBUG PORT
    push process
    call ecx              ; NtQueryInformationProcess(process, 7, &isDebuggerPresent, 4, NULL)

    cmp isDebuggerPresent, 0
    pop eax
    pop ecx
    jne debuggerfound
  }
  printf("Find me\n");
  return 0;
debuggerfound:
  printf("Stop using debugger\n");
  return 0;
}
