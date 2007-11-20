// UrModelViewer.cpp : Defines the entry point for the application.
//

#include "Core.h"
#include "UrModelViewer.h"
#include "RenderInterface.h"
#include "UrsaGL.h"
#include "UThreading.h"
#include "UrRender.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;
char* szTitle = "Model Viewer";					// The title bar text
char* szWindowClass = "ModelView";			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void CreatePixelFormat(PIXELFORMATDESCRIPTOR& pfd)
{
	uint ColorBits = 32;
	uint DepthBits = 16;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = ColorBits;
	pfd.cRedBits = 0;
	pfd.cRedShift = 0;
	pfd.cGreenBits = 0;
	pfd.cGreenShift = 0;
	pfd.cBlueBits = 0;
	pfd.cBlueShift = 0;
	pfd.cAlphaBits = 0;
	pfd.cAlphaShift = 0;
	pfd.cAccumAlphaBits = 0;
	pfd.cAccumBits = 0;
	pfd.cAccumBlueBits = 0;
	pfd.cAccumGreenBits = 0;
	pfd.cAccumRedBits = 0;
	pfd.cDepthBits = DepthBits;
	pfd.cStencilBits = 0;
	pfd.cAuxBuffers = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.bReserved = 0;
	pfd.dwLayerMask = 0;
	pfd.dwVisibleMask = 0;
	pfd.dwDamageMask = 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;

	// Initialize global strings
	MyRegisterClass(hInstance);

	KThreadManager::Initialize();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	HDC hDC = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd;

	CreatePixelFormat(pfd);

	int PixelFormat=ChoosePixelFormat(hDC, &pfd);
	assert(PixelFormat);
	bool success = SetPixelFormat(hDC, PixelFormat, &pfd);
	assert(success);

	HWindowContext WinContext(hDC);
	UrRenderThread* RenderThread = new UrRenderThread(WinContext); 
	GThreadManager->CreateThread(RenderThread, Thread_Rendering);
	UrRenderer* Renderer = RenderThread->GetRenderer();

	bool bContinue = true;

	RenderCommand(Renderer, new UrClearColorCommand(KColor(75, 75, 75, 255)));
	RenderCommand(Renderer, new UrCreateGridCommand(KColor(0, 0, 255, 255), 5, 0.2f));
	// Main message loop:
	while(bContinue)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message == WM_QUIT)
				bContinue = false;
			else if(msg.message == WM_PAINT)
				break;
		}
		if(RenderGameSync(Renderer))
		{
			RenderCommand(Renderer, new UrRenderCommand(Render_Command_FrameSync));
		}
	}
	RenderCommand(Renderer, new UrRenderCommand(Render_Command_Kill));

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_URMODELVIEWER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_URMODELVIEWER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		//EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
