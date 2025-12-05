#ifndef CPU_BURNER_H_
#define CPU_BURNER_H_

// This file is for separating out declarations from the main application about_win.cc file
// Use it for forward declaring the main window processing functions, extern keywords for
// handles, and system DLL pointer types.

#include "framework.h"

#include "resource.h"

// Maximum size of string that can be loaded from resource table with LoadString().
#define MAX_LOADSTRING 255

/* Global Variables */

// current instance
extern HINSTANCE hInst;

extern HWND hEdit;

// Dummy file output for conhost
static FILE* fDummyFile;

// The title bar text
static const WCHAR* CAPTION_TITLE = L"CPU Burner";
static const WCHAR* START_TITLE = L"Start Threads";
static const WCHAR* STOP_TITLE = L"Stop Threads";

// The main window class name
static WCHAR szWindowClass[MAX_LOADSTRING];

/* End of global variables */

/* Forward declarations of functions included in this translation unit. */

// Forward declarations

void OnStartButtonClick(HWND hWnd);

void OnStopButtonClick(HWND hWnd);

// Creates the main window with CreateWindowW()
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

// Window procedure function https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-wndproc
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Registers the window class.
ATOM RegisterWndClass(HINSTANCE hInstance);

// Message handler for the "About" dialog box.
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

#endif // CPU_BURNER_H_
