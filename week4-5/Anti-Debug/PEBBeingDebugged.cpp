#include <cstdio>


int main(int argc, char** argv) {
  __asm {
    mov eax, dword ptr fs:[0x30]
    movzx eax, byte ptr [eax+0x02]
    test eax, eax
    jnz debuggerfound
  }
  printf("Find me\n");
  return 0;
debuggerfound:
  printf("Stop using debugger\n");
  return 0;
}
