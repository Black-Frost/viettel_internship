#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

HANDLE FindProcess(PCSTR process) {
  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (Process32First(snapshot, &entry) != TRUE)
    return NULL;

  while (Process32Next(snapshot, &entry) == TRUE) {
    if (stricmp(entry.szExeFile, process) == 0) {
	  printf("Found Explorer.exe with pid: %d\n", entry.th32ProcessID);
      return OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
    }
  }
  return NULL;
}

int main(int argc, char** argv) {
  LPCSTR DllPath = "E:\\internship\\out\\build\\bin\\sss.dll";
  HANDLE hProcess = FindProcess("explorer.exe");
  LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
  WriteProcessMemory(hProcess, pDllPath, (LPVOID) DllPath, strlen(DllPath) + 1, 0);

  DWORD threadId;
  HANDLE hLoadThread = CreateRemoteThread(
      hProcess, 0, 0,
      (LPTHREAD_START_ROUTINE) GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"),
      pDllPath, 0, &threadId
  );

  if (hLoadThread == NULL) {
    printf("Error Create Remote Thread\n");
    return 1;
  }

  printf("Successfully create thread with id %d\n", threadId);

  // WaitForSingleObject(hLoadThread, INFINITE);

  // printf("DLL Load into Explorer.exe at %X\n", pDllPath);

  VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);

}
