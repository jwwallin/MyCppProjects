#pragma once
#include "MyIncludes.h"

class Mesh;
class Camera;

class Device
{
public:
	Device(COLORREF * buffer, int x, int y);

	void Clear(unsigned char r, unsigned char g, unsigned char b);
	void PutPixel(int x, int y, COLORREF color);
	DirectX::SimpleMath::Vector2 Project(DirectX::SimpleMath::Vector3 coord, DirectX::SimpleMath::Matrix TransMat);
	void DrawPoint(DirectX::SimpleMath::Vector2 point);
	void DrawLine(DirectX::SimpleMath::Vector2, DirectX::SimpleMath::Vector2);
	void Render(Camera *camera, std::vector <Mesh*> meshes);


private:
	COLORREF			*	buffer;
	int						size_x;
	int						size_y;
};

