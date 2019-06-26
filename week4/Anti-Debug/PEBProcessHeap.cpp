#include <cstdio>

int main(int argc, char** argv) {
  __asm {
    mov eax, dword ptr fs:[0x30]
    mov eax, dword ptr [eax+0x18]
    cmp dword ptr [eax+0x44], 0     ; ForceFlags
    jne debuggerfound
    cmp dword ptr [eax+0x40], 0     ; Flags
    jne debuggerfound
  }
  printf("Find me\n");
  return 0;
debuggerfound:
  printf("Stop using debugger\n");
  return 0;
}
