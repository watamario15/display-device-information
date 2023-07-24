#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#ifndef NOMIMNAX
#define NOMIMNAX
#endif

#include <math.h>
#include <tchar.h>
#include <windows.h>

#ifdef _MSC_VER
#define WSTR_FMT L"s"
#else
#define wcscat_s(dest, size_dummy, src) wcscat(dest, src)
#define swprintf_s swprintf
#define WSTR_FMT L"S"
#endif

// The function pointer type for TaskDialog API.
typedef HRESULT(__stdcall *TaskDialog_t)(HWND hwndOwner, HINSTANCE hInstance, const wchar_t *pszWindowTitle,
                                         const wchar_t *pszMainInstruction, const wchar_t *pszContent,
                                         int dwCommonButtons, const wchar_t *pszIcon, int *pnButton);

int messageBox(HWND hWnd, HINSTANCE hInst, const wchar_t *lpText, const wchar_t *lpCaption, unsigned uType) {
  // Tests whether uType uses some features that TaskDialog doesn't support.
  if (uType & ~(MB_ICONMASK | MB_TYPEMASK)) goto mbfallback;

  int buttons;
  switch (uType & MB_TYPEMASK) {
    case MB_OK:
      buttons = 1;
      break;
    case MB_OKCANCEL:
      buttons = 1 + 8;
      break;
    case MB_RETRYCANCEL:
      buttons = 16 + 8;
      break;
    case MB_YESNO:
      buttons = 2 + 4;
      break;
    case MB_YESNOCANCEL:
      buttons = 2 + 4 + 8;
      break;
    default:  // Not supported by TaskDialog.
      goto mbfallback;
  }

  wchar_t *icon;
  switch (uType & MB_ICONMASK) {
    case 0:
      icon = NULL;
      break;
    case MB_ICONWARNING:  // Same value as MB_ICONEXCLAMATION.
      icon = MAKEINTRESOURCEW(-1);
      break;
    case MB_ICONERROR:  // Same value as MB_ICONSTOP and MB_ICONHAND.
      icon = MAKEINTRESOURCEW(-2);
      break;
    default:  // Substitute anything else for the information icon.
      icon = MAKEINTRESOURCEW(-3);
  }

  {
    // Tries to load the TaskDialog API which is a newer substitite of MessageBoxW.
    // This API is per monitor DPI aware but doesn't exist before Windows Vista.
    // To make the system select version 6 comctl32.dll, we don't use the full path here.
    HMODULE comctl32 = LoadLibraryW(L"comctl32.dll");
    if (!comctl32) goto mbfallback;

    TaskDialog_t taskDialog = (TaskDialog_t)(void *)GetProcAddress(comctl32, "TaskDialog");
    if (!taskDialog) {
      FreeLibrary(comctl32);
      goto mbfallback;
    }

    int result;
    taskDialog(hWnd, hInst, lpCaption, L"", lpText, buttons, icon, &result);
    FreeLibrary(comctl32);
    return result;
  }

mbfallback:
  return MessageBoxW(hWnd, lpText, lpCaption, uType);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wchar_t *lpCmdLine, int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

  static wchar_t text[65536] = {0};
  int chars = 0;

  // Obtains each display device information.
  for (int i = 0;; ++i) {
    DISPLAY_DEVICEW device, monitor;
    ZeroMemory(&device, sizeof(DISPLAY_DEVICEW));
    ZeroMemory(&monitor, sizeof(DISPLAY_DEVICEW));
    device.cb = monitor.cb = sizeof(DISPLAY_DEVICEW);

    if (!EnumDisplayDevicesW(NULL, i, &device, 0)) break;
    if (!(device.StateFlags & DISPLAY_DEVICE_ACTIVE)) continue;  // Ignores inactive devices.
    EnumDisplayDevicesW(device.DeviceName, 0, &monitor, 0);      // Obtains the monitor information.
    const HDC hdc = CreateDCW(device.DeviceName, device.DeviceName, NULL, NULL);
    const double width = GetDeviceCaps(hdc, HORZSIZE) / 10.0, height = GetDeviceCaps(hdc, VERTSIZE) / 10.0;
    const int wpx = GetDeviceCaps(hdc, HORZRES), hpx = GetDeviceCaps(hdc, VERTRES);
    const double inch = sqrt(width * width + height * height) / 2.54, dpi = sqrt(wpx * wpx + hpx * hpx) / inch;
    DeleteDC(hdc);

    if (i >= 1) {
      if (sizeof(text) / sizeof(text[0]) - chars <= 4) {
        messageBox(NULL, hInstance, L"An error occurred.", L"Error", MB_OK | MB_ICONERROR);
        return EXIT_FAILURE;
      }
      wcscat_s(text, sizeof(text) / sizeof(text[0]), L"\r\n\r\n");
      chars += 4;
    }

    const int result = swprintf_s(text + chars, sizeof(text) / sizeof(text[0]) - chars,
                                  L"Device Name: %" WSTR_FMT
                                  L"\r\nResolution: %dx%d\r\nHeight: %.1f cm, Width: "
                                  L"%.1f cm\r\nDisplay size: %.1f inch\r\ndpi: %.1f dpi",
                                  monitor.DeviceString, hpx, wpx, height, width, inch, dpi);
    if (result <= 0) {
      messageBox(NULL, hInstance, L"An error occurred.", L"Error", MB_OK | MB_ICONERROR);
      return EXIT_FAILURE;
    }
    chars += result;
  }

  return messageBox(NULL, hInstance, text, L"Display Device Information", MB_OK | MB_ICONINFORMATION);
}
