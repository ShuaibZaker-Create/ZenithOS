#include <Windows.h>
#include <Shobjidl.h> // Include Shell API header

int main() {
    // Initialize COM library
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        // Handle initialization failure
        return 1;
    }

    // Open a file explorer window
    hr = CoCreateInstance(CLSID_ShellApplication, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(NULL));
    if (FAILED(hr)) {
        // Handle creation failure
        CoUninitialize(); // Release COM library
        return 1;
    }

    // Release COM library
    CoUninitialize();

    return 0;
}
