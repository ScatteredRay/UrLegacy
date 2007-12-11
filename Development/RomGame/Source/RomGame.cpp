#include "Core.h"
#include "RomGame.h"
#include "RenderInterface.h"
#include "UThreading.h"
#include "UrRender.h"
#include "UInput.h"
#include "RomPlayer.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;								// current instance
HWND hWnd;
char* szTitle = "Romance";					// The title bar text
char* szWindowClass = "RomGame";			// the main window class name

int OldXPos;
int OldYPos;
float CameraYaw;
float CameraPitch;
float CameraDist;

//Temp Input vars
float aForward;
float aBackwards;
float aLeft;
float aRight;
float aUp;
float aDown;
float aRollLeft;
float aRollRight;

UInput* GInput;
RomPlayer* GLocalPlayer;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#if USING_GL
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
#endif //USING_GL

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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

#if USING_GL
	PIXELFORMATDESCRIPTOR pfd;

	CreatePixelFormat(pfd);

	int PixelFormat=ChoosePixelFormat(hDC, &pfd);
	assert(PixelFormat);
	bool success = SetPixelFormat(hDC, PixelFormat, &pfd);
	assert(success);
#endif //USING_GL
#if USING_DX
	gD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
#endif //USING_DX

	HWindowContext WinContext(hWnd);
	UrRenderThread* RenderThread = new UrRenderThread(WinContext); 
	GThreadManager->CreateThread(RenderThread, Thread_Rendering);
	UrRenderer* Renderer = RenderThread->GetRenderer();

	OldXPos = 0;
	OldYPos = 0;

	CameraYaw = 0.0f;
	CameraPitch = 0.0f;
	CameraDist = 2.0f;

	bool bContinue = true;

	RCClearColor(Renderer, KColor(75, 75, 75, 255));
	//RenderCommand(Renderer, new UrCreateGridCommand(KColor(0, 0, 255, 255), 10, 0.5f));
	RCCreateGrid(Renderer, KColor(0, 0, 255, 255), 10, 0.5f);
	RCCameraProjection(Renderer, 1.0f, 100.0f, 1.6f, 1.6f);

	GObjectManager = new UObjectManager();
	GInput = new UInput();

	aForward = aBackwards = aLeft = aRight = aUp = aDown = aRollLeft = aRollRight = 0.0f; //Temp awaiting better input system.

	GLocalPlayer = new RomPlayer();
	GLocalPlayer->RegisterInput(GInput);

	uint64 PerformanceFrequency;
	uint64 LastFrameTime;
	uint64 FrameTime;

	QueryPerformanceFrequency((LARGE_INTEGER*)&PerformanceFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&LastFrameTime);

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
			float FrameDelta;
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&FrameTime);
				uint64 DeltaCounter = FrameTime - LastFrameTime;
				FrameDelta = (float)(((double)DeltaCounter)/((double)PerformanceFrequency));
				LastFrameTime = FrameTime;
			}
			GObjectManager->TickObjects(FrameDelta);

			KMatrix ViewMatrix;
			KMatrix ZoomMat = MatrixTranslate(KVector(0.0f, 0.0f, CameraDist));
			GLocalPlayer->GenerateViewMatrix(&ViewMatrix);
			RCViewTransform(Renderer, ZoomMat*ViewMatrix);

			RenderCommand(Renderer, new UrRenderCommand(Render_Command_FrameSync));
		}
	}
	RenderCommand(Renderer, new UrRenderCommand(Render_Command_Kill));

	delete GLocalPlayer;

#if USING_DX
	gD3D->Release();
#endif //USING_DX

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
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);

			GInput->SetAxisInput(Name("PlayerYaw"), (xPos - OldXPos)/512.0f);
			GInput->SetAxisInput(Name("PlayerPitch"), (OldYPos - yPos)/512.0f);

			OldXPos = xPos;
			OldYPos = yPos;
		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'e':
		case 'E':
			aUp = 1.0f;
			break;
		case 'd':
		case 'D':
			aDown = 1.0f;
			break;
		case 's':
		case 'S':
			aLeft = 1.0f;
			break;
		case 'f':
		case 'F':
			aRight = 1.0f;
			break;
		case 'w':
		case 'W':
			aRollLeft = 1.0f;
			break;
		case 'r':
		case 'R':
			aRollRight = 1.0f;
			break;
		case 'a':
		case 'A':
			aForward = 1.0f;
			break;
		case 'z':
		case 'Z':
			aBackwards = 1.0f;
		}
		goto AccumulateAxis;
	case WM_KEYUP:
		switch(wParam)
		{
		case 'e':
		case 'E':
			aUp = 0.0f;
			break;
		case 'd':
		case 'D':
			aDown = 0.0f;
			break;
		case 's':
		case 'S':
			aLeft = 0.0f;
			break;
		case 'f':
		case 'F':
			aRight = 0.0f;
			break;
		case 'w':
		case 'W':
			aRollLeft = 0.0f;
			break;
		case 'r':
		case 'R':
			aRollRight = 0.0f;
			break;
		case 'a':
		case 'A':
			aForward = 0.0f;
			break;
		case 'z':
		case 'Z':
			aBackwards = 0.0f;
		}
		goto AccumulateAxis;
	AccumulateAxis:
		GInput->SetAxisInput(Name("PlayerRoll"), aRollRight - aRollLeft);
		GInput->SetAxisInput(Name("PlayerThrottle"), aForward - aBackwards);
		GInput->SetAxisInput(Name("PlayerSlide"), aRight - aLeft);
		GInput->SetAxisInput(Name("PlayerRise"), aUp - aDown);
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
