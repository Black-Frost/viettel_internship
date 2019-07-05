#include <cstdio>


int main(int argc, char** argv) {
  __asm {
    mov eax, dword ptr fs:[0x30]      ; PEB
    cmp dword ptr [eax+0x68], 0x70    ; PEB.NtGlobalFlag
    je debuggerfound
  }
  printf("Find me\n");
  return 0;
debuggerfound:
  printf("Stop using debugger");
  return 0;
}
