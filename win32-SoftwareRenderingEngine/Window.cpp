
#include "Window.h"
#include "MyIncludes.h"
#include <Windows.h>
#include <assert.h>





Window::Window(/*Renderer * renderer,*/ unsigned int size_x, unsigned int size_y, std::string name, COLORREF *buffer)
{
	/*this->renderer = renderer;*/
	this->surface_size_x = size_x;
	this->surface_size_y = size_y;
	this->window_name = std::wstring(name.begin(), name.end());

	this->buffer = buffer;

	InitOSWindow();
}

Window::~Window()
{
	DeInitOSWindow();
}

void Window::Close()
{
	window_should_run = false;
}

bool Window::Update()
{
	//UpdateWindow(win32_window);
	RedrawWindow(win32_window, NULL, NULL, RDW_INVALIDATE);

	//update window based on callbacks
	UpdateOSWindow();
	return window_should_run;
}

uint32_t Window::GetSurfaceSizeX()
{
	return surface_size_x;
}

uint32_t Window::GetSurfaceSizeY()
{
	return surface_size_y;
}

HWND Window::GetWindowHandle()
{
	return win32_window;
}

COLORREF * Window::GetMyBuffer()
{
	return buffer;
}

PAINTSTRUCT 	ps;
HDC				hdc;
HBITMAP			map;
BITMAP 			bitmap;
HDC 			hdcMem;
HGDIOBJ 		oldBitmap;

unsigned int size_x;
unsigned int size_y;

LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window * window = reinterpret_cast<Window*>(
		GetWindowLongPtrW(hWnd, GWLP_USERDATA));


	switch (uMsg) {
	case WM_PAINT:
		size_x = window->GetSurfaceSizeX();
		size_y = window->GetSurfaceSizeY();

		hdc = BeginPaint(window->GetWindowHandle(), &ps);
		//draw rendered bitmap
		map = CreateBitmap(size_x, size_y,
			1,
			8 * 4,
			(void*)window->GetMyBuffer());

		hdcMem = CreateCompatibleDC(hdc);
		oldBitmap = SelectObject(hdcMem, map);

		GetObject(map, sizeof(bitmap), &bitmap);
		BitBlt(hdc,
			0, 0,
			bitmap.bmWidth, bitmap.bmHeight,
			hdcMem,
			0, 0,
			SRCCOPY);


		SelectObject(hdcMem, oldBitmap);


		DeleteDC(hdcMem);
		EndPaint(window->GetWindowHandle(), &ps);
		DeleteObject(map);

		break;
	case WM_CLOSE:
		window->Close();
		return 0;
	case WM_SIZE:
		// we get here if the window has changed size, we should rebuild most
		// of our window resources before rendering to this window again.
		// ( no need for this because our window sizing by hand is disabled )
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::InitOSWindow()
{
	WNDCLASSEX win_class{};
	assert(surface_size_x > 0);
	assert(surface_size_y > 0);

	win32_instance = GetModuleHandle(nullptr);
	win32_class_name = window_name;
	win32_class_id_counter++;

	// Initialize the window class structure:
	win_class.cbSize = sizeof(WNDCLASSEX);
	win_class.style = CS_HREDRAW | CS_VREDRAW;
	win_class.lpfnWndProc = WindowsEventHandler;
	win_class.cbClsExtra = 0;
	win_class.cbWndExtra = 0;
	win_class.hInstance = win32_instance; // hInstance
	win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	win_class.lpszMenuName = NULL;
	win_class.lpszClassName = win32_class_name.c_str();
	win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	// Register window class:
	if (!RegisterClassEx(&win_class)) {
		// It didn't work, so try to give a useful error:
		assert(0 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}

	DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	// Create window with the registered class:
	RECT wr = { 0, 0, LONG(surface_size_x), LONG(surface_size_y) };
	AdjustWindowRectEx(&wr, style, FALSE, ex_style);
	win32_window = CreateWindowEx(0,
		win32_class_name.c_str(),		// class name
		window_name.c_str(),			// app name
		style,							// window style
		CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
		wr.right - wr.left,				// width
		wr.bottom - wr.top,				// height
		NULL,							// handle to parent
		NULL,							// handle to menu
		win32_instance,				// hInstance
		NULL);							// no extra parameters
	if (!win32_window) {
		// It didn't work, so try to give a useful error:
		assert(1 && "Cannot create a window in which to draw!\n");
		fflush(stdout);
		std::exit(-1);
	}
	SetWindowLongPtr(win32_window, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(win32_window, SW_SHOW);
	SetForegroundWindow(win32_window);
	SetFocus(win32_window);
}

void Window::DeInitOSWindow()
{
	DestroyWindow(win32_window);
	UnregisterClass(win32_class_name.c_str(), win32_instance);
}

void Window::UpdateOSWindow()
{
	MSG msg;
	if (PeekMessage(&msg, win32_window, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


