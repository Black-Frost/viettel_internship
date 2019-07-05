#include <cstdio>
#include <windows.h>

int main(int argc, char** argv) {
  BOOL isDebuggerPresent = FALSE;
  CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent);
  if (isDebuggerPresent) {
    printf("Stop usng debugger\n");
    return 0;
  }
  printf("Find me\n");
  return 0;
}
