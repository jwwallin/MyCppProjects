#pragma once
#include "MyIncludes.h"
class Window
{
public:
	Window(/*Renderer * renderer,*/ unsigned int size_x, unsigned int size_y, std::string name, COLORREF *buffer);
	~Window();

	void Close();
	bool Update();

	unsigned int GetSurfaceSizeX();
	unsigned int GetSurfaceSizeY();
	void GetRenderedBitmap();

	HWND GetWindowHandle();
	COLORREF* GetMyBuffer();

private:
	void								InitOSWindow();
	void								DeInitOSWindow();
	void								UpdateOSWindow();
	void								InitOSSurface();

	void								InitSurface();
	void								DeInitSurface();



	/*Renderer						*	renderer = nullptr;*/

	unsigned int						surface_size_x = 600;
	unsigned int						surface_size_y = 400;
	std::wstring						window_name;

	bool								window_should_run = true;


	HINSTANCE							win32_instance = 0;
	std::wstring						win32_class_name;
	HWND								win32_window = 0;
	unsigned int						win32_class_id_counter;
	HDC									win32_device_context;

	COLORREF						*	buffer;
};

