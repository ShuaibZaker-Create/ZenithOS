#include <Windows.h>
#include <ShObjIdl.h> // Include Shell API header
#include <CommCtrl.h> // Include Common Controls header
#include <vector>
#include <string>

// Global variables
HWND g_hWndListView;
std::vector<std::wstring> g_fileList;

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void OnButtonClick();
void PopulateListView(const std::wstring& folderPath);
void AddListViewItem(const std::wstring& fileName);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0,
                      GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
                      L"FileExplorerWindowClass", NULL };
    RegisterClassEx(&wc);

    // Create main window
    HWND hWnd = CreateWindowEx(0, L"FileExplorerWindowClass", L"File Explorer", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, NULL, NULL, hInstance, NULL);
    if (!hWnd) {
        return -1;
    }

    // Create button
    HWND hWndButton = CreateWindow(L"BUTTON", L"Open Folder", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                   10, 10, 100, 30, hWnd, NULL, hInstance, NULL);

    // Create list view
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    g_hWndListView = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
                                  10, 50, 560, 300, hWnd, NULL, hInstance, NULL);

    // Set list view style
    ListView_SetExtendedListViewStyle(g_hWndListView, LVS_EX_FULLROWSELECT);

    // Set list view columns
    LVCOLUMN lvColumn;
    lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
    lvColumn.cx = 200;
    lvColumn.pszText = L"File Name";
    ListView_InsertColumn(g_hWndListView, 0, &lvColumn);

    // Show main window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_COMMAND:
            if (LOWORD(wParam) == BN_CLICKED && (HWND)lParam == g_hWndButton) {
                OnButtonClick();
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OnButtonClick() {
    // Initialize COM library
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        // Handle initialization failure
        return;
    }

    // Open a file dialog to let the user select a folder
    IFileDialog* pFileDialog;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (FAILED(hr)) {
        // Handle creation failure
        CoUninitialize(); // Release COM library
        return;
    }

    // Set options for the file dialog
    DWORD dwOptions;
    pFileDialog->GetOptions(&dwOptions);
    pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

    // Show the file dialog
    hr = pFileDialog->Show(NULL);
    if (FAILED(hr)) {
        // Handle dialog display failure
        pFileDialog->Release();
        CoUninitialize(); // Release COM library
        return;
    }

    // Get the selected folder
    IShellItem* pItem;
    hr = pFileDialog->GetResult(&pItem);
    if (FAILED(hr)) {
        // Handle folder retrieval failure
        pFileDialog->Release();
        CoUninitialize(); // Release COM library
        return;
    }

    // Get the folder path
    LPWSTR pszFolderPath;
    pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);

    // Populate list view with folder contents
    PopulateListView(pszFolderPath);

    // Release resources
    CoTaskMemFree(pszFolderPath);
    pItem->Release();
    pFileDialog->Release();

    // Release COM library
    CoUninitialize();
}

void PopulateListView(const std::wstring& folderPath) {
    // Clear existing list view items
    ListView_DeleteAllItems(g_hWndListView);
    g_fileList.clear();

    // Add list view items for files in the folder
    WIN32_FIND_DATAW findData;
    HANDLE hFind = FindFirstFileW((folderPath + L"\\*").c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                AddListViewItem(findData.cFileName);
            }
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    }
}

void AddListViewItem(const std::wstring& fileName) {
    LVITEM lvItem;
    lvItem.mask = LVIF_TEXT;
    lvItem.iItem = g_fileList.size();
    lvItem.iSubItem = 0;
    lvItem.pszText = (LPWSTR)fileName.c_str();

    ListView_InsertItem(g_hWndListView, &lvItem);
    g_fileList.push_back(fileName);
}
