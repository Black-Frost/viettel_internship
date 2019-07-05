#include <cstdio>
#include <windows.h>


int main(int argc, char** argv) {
  BOOL debugger = IsDebuggerPresent();
  if (debugger) {
    printf("Stop using debugger\n");
    return 0;
  }
  printf("Find me\n");
  return 0;
}
