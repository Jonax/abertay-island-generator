//////////////////////////////////////////////////////////////////////////////////////////
//						  Dynamic Terrain Generation Prototype							//
//					  Written 2007 by Jon Wills (jc@chromaticaura.net)						//
//				Written for a Win32 environment using the DirectSound API.				//
//																						//
//				   Written at the University of Abertay Dundee, Scotland				//
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	APPLICATION TOP LEVEL																//
//	The root file of the program.														//
//////////////////////////////////////////////////////////////////////////////////////////

#include "Include.h"
#include "Win32Setup.h"
#include "OpenGL.h"

HINSTANCE	ghInstance;
RECT		screenRect;

POINT MousePos;

bool		keys[256];

bool MouseLock;

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)											
	{
		case WM_SIZE:
			Renderer.ResizeGLWindow(LOWORD(lParam), HIWORD(lParam));
			GetClientRect(hwnd, &Renderer.gRect);
			break;	

		case WM_KEYDOWN:
			keys[wParam] = true;
			switch(wParam)
			{
				// Controls for the game.  
				
				// 1-2 - Switch cameras.  
				case '1':
					Renderer.god_mode = true;
					break;
				case '2':
					Renderer.god_mode = false;
					break;

				// Arrow keys - Moves camera in First Person mode.  
				case VK_UP:
					Renderer.MoveCamera(1.0f);
					break;
				case VK_DOWN:
					Renderer.MoveCamera(-1.0f);
					break;
				case VK_LEFT:
					Renderer.StrafeCam(-1.0f);
					break;
				case VK_RIGHT:
					Renderer.StrafeCam(1.0f);
					break;
				
				// N - Main Generation Algorithm.  
				case 'n':
				case 'N':
					Renderer.Terrain->GeneratePerlinNoise(10);
					Renderer.Terrain->IslandifyByBellCurve();
					break;

				// Q - One Perlin Pass.
				case 'q':
				case 'Q':
					Renderer.Terrain->GeneratePerlinNoise(1);
					break;
				// W - One Islandify Pass.  
				case 'w':
				case 'W':
					Renderer.Terrain->IslandifyByBellCurve();
					break;
				// E - Generates a bell-curve.  
				case 'e':
				case 'E':
					Renderer.Terrain->GenerateBellCurve();
					break;
			}
			break;

		case WM_KEYUP:
			keys[wParam] = false;
			break;

		case WM_MOUSEMOVE:
			MousePos.x = LOWORD(lParam);
			MousePos.y = HIWORD(lParam);
			break;

		// Left Mouse Button - Rotation Lock.  
		case WM_LBUTTONDOWN:
			Renderer.GodCam->SetRotate(false);
			break;
		case WM_LBUTTONUP:
			Renderer.GodCam->SetRotate(true);
			break;

		// Right Mouse Button - Zoom Lock.  
		case WM_RBUTTONDOWN:
			Renderer.GodCam->SetZoom(false);
			break;
		case WM_RBUTTONUP:
			Renderer.GodCam->SetZoom(true);
			break;

		case WM_DESTROY:	
			PostQuitMessage(0);
			break;
	}													

	return DefWindowProc (hwnd, message, wParam, lParam);															
}

HWND CreateOurWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hwnd;
	WNDCLASS wcex;

	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= WndProc;		
	wcex.hInstance		= hInstance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);; 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= "FirstWindowClass";	

	RegisterClass(&wcex);// Register the class

	dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	ghInstance	= hInstance;// Assign our global hInstance to the window's hInstance

	//Set the Client area of the window to be our resolution.
	RECT glwindow;
	glwindow.left		= 0;		
	glwindow.right		= width;	
	glwindow.top		= 0;		
	glwindow.bottom		= height;	

	AdjustWindowRect( &glwindow, dwStyle, false);

	//Create the window
	hwnd = CreateWindow(	"FirstWindowClass", 
							strWindowName, 
							dwStyle, 
							0, 
							0,
							glwindow.right  - glwindow.left,
							glwindow.bottom - glwindow.top, 
							NULL,
							NULL,
							hInstance,
							NULL
							);

	if(!hwnd) return NULL;// If we could get a handle, return NULL

	ShowWindow(hwnd, SW_SHOWNORMAL);	
	UpdateWindow(hwnd);					
	SetFocus(hwnd);						

	return hwnd;
}

void Cleanup()
{
	Renderer.Cleanup();

	UnregisterClass("FirstWindowClass", ghInstance);// Free the window class

	PostQuitMessage(0);		// Post a QUIT message to the window
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int nCmdShow)			
{	
	HWND		hwnd;
    MSG         msg;	

	//initialise and create window
	hwnd = CreateOurWindow("Dynamic Terrain Generation Prototype", S_WIDTH, S_HEIGHT, 0, false, hInstance);

	if (hwnd == NULL) 
		return true;

	//initialise opengl and other settings
	Renderer.Initialise(hwnd);
	
	while (true)					
    {							
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
		    if (msg.message == WM_QUIT)
				break;

			TranslateMessage (&msg);							
			DispatchMessage (&msg);
		}
		else
		{
			// Main rendering loop.  
			Renderer.ProcessMouseInput(MousePos);	// Processes mouse input at the start of the frame...
			Renderer.DrawScene();					// ...then drawing the scene...
			Renderer.ResetCamera(MousePos);			// ...and then finally recording the old mouse position at the end.  
		}
    }

	return msg.wParam ;										
}