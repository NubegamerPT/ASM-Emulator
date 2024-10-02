#include <windows.h>

// Declare the window procedure function
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Entry point for a Windows application
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"SampleWindowClass";

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    // Register the window class
    if (!RegisterClassExW(&wc)) {
        MessageBoxW(NULL, L"Failed to register window class!", L"Error", MB_ICONERROR);
        return 0;
    }

    // Create the main window
    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Raw C++ Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 2000, 900,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) {
        MessageBoxW(NULL, L"Failed to create window!", L"Error", MB_ICONERROR);
        return 0;
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Window procedure function
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hPanel1, hPanel2; // Static controls to simulate panels

    switch (msg) {
        case WM_CREATE: {
            // Create two static controls (acting as panels)
            hPanel1 = CreateWindowExW(
                0, L"STATIC", NULL,
                WS_CHILD | WS_VISIBLE | SS_GRAYFRAME, // Gray frame style to look like a panel
                50, 50, 400, 200,                    // Position and size of the first panel
                hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL
            );

            hPanel2 = CreateWindowExW(
                0, L"STATIC", NULL,
                WS_CHILD | WS_VISIBLE | SS_GRAYFRAME, // Another panel
                500, 50, 400, 200,                   // Position and size of the second panel
                hwnd, NULL, ((LPCREATESTRUCT)lParam)->hInstance, NULL
            );
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}