// {{NO_DEPENDENCIES}}
// For #define-ing static resources for resource script file(s).
// Used by resource.rc

// Icons
#define IDI_CPUBURNER			          101
#define IDI_SMALL				            102

// Main application resource
#define IDC_CPUBURNER			          103

// Dialogs
#define IDD_ABOUT_DIALOG  	        104
#define IDD_ABOUTBOX			          105

// Menu items
#define IDM_ABOUT				            200
#define IDM_EXIT				            201

// Controls
#define IDC_NUMTHREADS  	          300
#define IDC_BUTTON1   			        301
#define IDC_BUTTON2   			        302

// Static values
#define CW_MAINWIDTH             400
#define CW_MAINHEIGHT            200

// For resources to be loaded without an ID from the system.
#ifndef IDC_STATIC
 #define IDC_STATIC				         -1
#endif // IDC_STATIC

#define APP_MANIFEST                1
