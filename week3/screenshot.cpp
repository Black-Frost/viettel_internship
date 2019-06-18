#include <windows.h>


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
  GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, &st, "'screenshot_'HH'_'mm'_'ss'.bmp'", filename, 30);
  hf = CreateFile(
      filename,
      GENERIC_READ | GENERIC_WRITE,
      (DWORD) 0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      (HANDLE) NULL
  );
  delete filename;

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


int WINAPI
wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR pCmdLine,
    int nCmdShow
) {

  HWND hwnd = 0;
  MSG msg;

  UINT min = 60 * 1000;

  UINT_PTR timer = SetTimer(hwnd, 0, 0, (TIMERPROC) screenshot);

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);

    if (msg.message == WM_TIMER)
      timer = SetTimer(hwnd, 0, 2 * min, (TIMERPROC) screenshot);
  }

  return 0;
}
