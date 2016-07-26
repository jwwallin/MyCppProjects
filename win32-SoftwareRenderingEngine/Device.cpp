#include "Device.h"

using namespace DirectX::SimpleMath;

Device::Device(COLORREF * buffer, int x, int y)
{
	this->buffer = buffer;
	size_x = x;
	size_y = y;
}

void Device::Clear(unsigned char r, unsigned char g, unsigned char b)
{
	for (int index = 0; index < size_x*size_y; index++) {
		*(buffer + index) = RGB(r, g, b);
	}
}

void Device::PutPixel(int x, int y, COLORREF color)
{
	int index = x + y * size_x;

	*(buffer + index) = color;
}

Vector2 Device::Project(Vector3 coord, Matrix transMat)
{
	Vector3 point = Vector3::Transform(coord, transMat);

	int x = point.x * size_x + size_x / 2.0f;
	int y = -point.y * size_y + size_y / 2.0f;

	return Vector2(x,y);
}

void Device::DrawPoint(Vector2 point)
{
	if (point.x >= 0 && point.y >= 0 && point.x < size_x && point.y < size_y) {
		PutPixel((int)point.x, (int)point.y, RGB(255, 180, 180));
	}
}

void Device::DrawLine(Vector2 point1, Vector2 point2)
{
	int x0 = point1.x;
	int y0 = point1.y;
	int x1 = point2.x;
	int y1 = point2.y;

	auto dx = std::abs(x1 - x0);
	auto dy = std::abs(y1 - y0);
	auto sx = (x0 < x1) ? 1 : -1;
	auto sy = (y0 < y1) ? 1 : -1;
	auto err = dx - dy;

	while (true) {
		DrawPoint(Vector2(x0, y0));
		if ((x0 == x1) && (y0 == y1))break;
		auto e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}

}

void Device::Render(Camera* camera, std::vector<Mesh*> meshes )
{
	Matrix viewMatrix = Matrix::CreateLookAt((*camera).Position, 
												(*camera).Target, 
												Vector3::UnitY);
	Matrix projectionMatrix = Matrix::CreatePerspectiveFieldOfView(0.78f,						//fov
																	((float)size_x) / size_y,	//aspect ratio
																	0.01f, 1.0f);				//near and far planes

	for (Mesh* mesh: meshes) {

		Matrix modelMatrix = Matrix::CreateFromQuaternion((*mesh).Rotation) *
			Matrix::CreateTranslation((*mesh).Position);

		Matrix transformMatrix = modelMatrix * viewMatrix * projectionMatrix;

		for (Face face : (*mesh).Faces) {

			auto v1 = (*mesh).Vertices[face.A];
			auto v2 = (*mesh).Vertices[face.B];
			auto v3 = (*mesh).Vertices[face.C];

			auto p1 = Project(v1, transformMatrix);
			auto p2 = Project(v2, transformMatrix);
			auto p3 = Project(v3, transformMatrix);

			DrawLine(p1, p2);
			DrawLine(p1, p3);
			DrawLine(p2, p3);
		}
	}
}
