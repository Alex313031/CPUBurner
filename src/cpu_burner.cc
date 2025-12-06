#include "cpu_burner.h"
#include "stress.h"

// Global instance
HINSTANCE hInst;

HWND hEdit;  // Edit control handle
HWND hBut1;  // Button 1 handle
HWND hBut2;  // Button 2 handle

int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  // Initialize common controls
  InitCommonControls();

  // Allow and allocate conhost for cmd.exe logging window
  if (!AllocConsole()) {
    return 1;
  }

  // File handler pointer to a dummy file, possibly an actual logfile
  FILE* fNonExistFile = fDummyFile;
#ifndef __MINGW32__
  freopen_s(&fNonExistFile, "CONOUT$", "w", stdout); // Standard error
  freopen_s(&fNonExistFile, "CONOUT$", "w", stderr); // Standard out
#else
  // freopen_s doesn't exist in MinGW...
  fNonExistFile = freopen("CONOUT$", "w", stdout); // Standard error
  fNonExistFile = freopen("CONOUT$", "w", stderr); // Standard out
#endif // __MINGW32__
  if (!fNonExistFile) {
    return 1;
  }

  LoadStringW(hInstance, IDC_CPUBURNER, szWindowClass, MAX_LOADSTRING);

  // Register the window class
  RegisterWndClass(hInstance);

  if (!InitInstance(hInstance, nCmdShow)) {
    return 2;
  }

  // Load keyboard accelerators.
  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPUBURNER));

  // Message loop
  MSG msg;
  // Main window message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

ATOM RegisterWndClass(HINSTANCE hInstance) {
  // Declare and set size of this window class struct.
  WNDCLASSEXW wcex;
  wcex.cbSize = sizeof(WNDCLASSEX);

  // Set styles, icons, and window message handling function.  
  wcex.style          = CS_HREDRAW | CS_VREDRAW; // Drawing style
  wcex.lpfnWndProc    = WindowProc; // Window Procedure function
  wcex.cbClsExtra     = 0; // Extra bytes to add to end of this window class
  wcex.cbWndExtra     = 0; // Extra bytes to add to end hInstance
  wcex.hInstance      = hInstance; // This instance
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPUBURNER)); // Load our main app icon
  wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW); // Choose default cursor style to show
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1); // Choose window client area background color
  wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPUBURNER); // Attach menu to window
  wcex.lpszClassName  = szWindowClass; // Use our unique window class name
  wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // Load titlebar icon

  // Returns a "class atom", a win32 specific data type.
  return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;
  // Create the main window
  HWND hwnd = CreateWindowExW(WS_EX_WINDOWEDGE,
                              szWindowClass,
                              CAPTION_TITLE,
                              WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              CW_MAINWIDTH,
                              CW_MAINHEIGHT,
                              nullptr,
                              nullptr,
                              hInstance,
                              nullptr);

  if (!hwnd) {
    return false;
  } else {
    // Create the Edit control
    hEdit = CreateWindowEx(
        WS_EX_CLIENTEDGE, L"EDIT", L"8",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        50, 40, 300, 20,
        hwnd, (HMENU)IDC_NUMTHREADS, hInstance, nullptr
    );

    // Create the Start Button control
    CreateWindow(
        L"BUTTON", START_TITLE,
        WS_CHILD | WS_VISIBLE,
        75, 80, 100, 30,
        hwnd, (HMENU)IDC_BUTTON1, hInstance, nullptr
    );

    // Create the Stop Button control
    CreateWindow(
        L"BUTTON", STOP_TITLE,
        WS_CHILD | WS_VISIBLE,
        225, 80, 100, 30,
        hwnd, (HMENU)IDC_BUTTON2, hInstance, nullptr
    );

    // Show the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    return true;
  }
}

// Window procedure for handling messages
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      switch (wmId) {
        case IDC_NUMTHREADS:
          break;
        case IDC_BUTTON1:
          OnStartButtonClick(hWnd);
          break;
        case IDC_BUTTON2:
          OnStopButtonClick(hWnd);
          break;
        case IDM_ABOUT:
          // Show "About" dialog box
          DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;
        case IDM_EXIT:
          // Send WM_DESTROY message to close window 
          DestroyWindow(hWnd);
          break;
        default:
          return DefWindowProc(hWnd, uMsg, wParam, lParam);
      }
    } break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
  return 0;
}

static void OnStart(int num_threads) {
  set_run_state(true);
  static const int threads = num_threads;
  static constexpr unsigned long num_ms = 10000L;
  test_stress(threads, num_ms);
}

void OnStartButtonClick(HWND hWnd) {
  // Get the length of the text in the edit control
  DWORD dwSize = GetWindowTextLength(hEdit);
  int threads_to_start = 0;
  if (dwSize > 0) {
    // Create a buffer to store the text
    static wchar_t* buffer = new wchar_t[dwSize + 1]; // Allocate memory for the text

    // Get the text from the edit control
    GetWindowText(hEdit, buffer, dwSize + 1);
    std::wcout << L"hEdit content = " << buffer << std::endl;
    threads_to_start = std::stoi(buffer);
  } else {
    MessageBoxW(hWnd, L"No text entered!", L"Error", MB_OK | MB_ICONERROR);
  }
  OnStart(threads_to_start);
}

void OnStopButtonClick(HWND hWnd) {
  std::wcout << L"Stop button clicked" << std::endl;
  set_run_state(false);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);

  bool AboutHandled = false; // Stores status of whether dialog has been handled user-wise.
  switch (message) {
    case WM_INITDIALOG:
      // Showed the dialog
      AboutHandled = true;
      break;
    case WM_COMMAND:
      // Exit the dialog
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        if (EndDialog(hDlg, LOWORD(wParam))) {
          AboutHandled = true;
          return (INT_PTR)AboutHandled;
        } else {
          AboutHandled = false;
          break;
        }
      } break;
    default:
      AboutHandled = false;
      break;
  }

  // About dialog failed
  return (INT_PTR)AboutHandled;
}
