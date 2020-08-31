#include <windows.h>
#include <math.h>
#include <string>

WCHAR text[65536];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    const int cbSize = sizeof(DISPLAY_DEVICE);

    // 初期化
    int cnt=0;
    text[0]=L'\0';
    std::wstring wstr;
    DISPLAY_DEVICE devices, monitors;
    ZeroMemory(&devices, cbSize);
    ZeroMemory(&monitors, cbSize);
    devices.cb = cbSize;	monitors.cb = cbSize;

    // 各ディスプレイデバイスを取得
    for(int i=0; EnumDisplayDevices(NULL, i, &devices, 0); i++){
        if(!(devices.StateFlags & DISPLAY_DEVICE_ACTIVE)) continue; // アクティブなデバイス以外は無視                
        EnumDisplayDevices(devices.DeviceName, 0, &monitors, 0); // モニタ情報を取得

        // CreateDC の第1,2引数にデバイス名を指定してディスプレイコンテキストを取得
        HDC hdc = CreateDC(devices.DeviceName, devices.DeviceName, NULL, NULL);

        // HORZSIZE を指定してディスプレイの幅を取得(mm単位なので10で割ってcm単位に)
        DOUBLE width = GetDeviceCaps(hdc, HORZSIZE)/10.0;
        INT wpx = GetDeviceCaps(hdc, HORZRES);

        // VERTSIZE を指定してディスプレイの高さを取得(同上)
        DOUBLE height = GetDeviceCaps(hdc, VERTSIZE)/10.0;
        INT hpx = GetDeviceCaps(hdc, VERTRES);

        // inchの計算
        DOUBLE inch = sqrt(width*width + height*height)/2.54;

        // dpi計算
        DOUBLE dpi = sqrt(wpx*wpx+hpx*hpx)/inch;

        if(cnt>=1){
            wcscat_s(text, TEXT("\n\n"));
            wstr += text;
        }
        cnt++;
        
        swprintf_s(
            text,
            TEXT("Device Name: %s\n")
            TEXT("Resolution: %dx%d\n")
            TEXT("Height: %.1fcm, Width: %.1fcm\n")
            TEXT("Display size: %.1finch\n")
            TEXT("dpi: %.1fdpi"),
            monitors.DeviceString, hpx, wpx, height, width, inch, dpi
        );

        DeleteDC(hdc);
        devices.cb = cbSize; monitors.cb = cbSize;
    }
    wstr += text;
    MessageBox(NULL, wstr.c_str(), TEXT("Display Device Information"), MB_OK | MB_ICONINFORMATION);
    return 0;
}