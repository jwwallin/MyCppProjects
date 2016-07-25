#include "Device.h"

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

DirectX::SimpleMath::Vector2 Device::Project(DirectX::SimpleMath::Vector3 coord, DirectX::SimpleMath::Matrix transMat)
{
	DirectX::SimpleMath::Vector3 point = DirectX::SimpleMath::Vector3::Transform(coord, transMat);

	int x = point.x * size_x + size_x / 2.0f;
	int y = -point.y * size_y + size_y / 2.0f;

	return DirectX::SimpleMath::Vector2(x,y);
}

void Device::DrawPoint(DirectX::SimpleMath::Vector2 point)
{
	if (point.x >= 0 && point.y >= 0 && point.x < size_x && point.y < size_y) {
		PutPixel((int)point.x, (int)point.y, RGB(255, 180, 180));
	}
}

void Device::Render(Camera* camera, std::vector<Mesh*> meshes )
{
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt((*camera).Position, 
																						(*camera).Target, 
																						DirectX::SimpleMath::Vector3::UnitY);
	DirectX::SimpleMath::Matrix projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.78f, 
																												((float)size_x) / size_y, 
																												0.01f, 1.0f);

	for (Mesh* mesh: meshes) {

		DirectX::SimpleMath::Matrix modelMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion((*mesh).Rotation) *
			DirectX::SimpleMath::Matrix::CreateTranslation((*mesh).Position);

		//glm::mat4 transformMatrix = projectionMatrix * viewMatrix * modelMatrix;
		DirectX::SimpleMath::Matrix transformMatrix = modelMatrix * viewMatrix * projectionMatrix;

		for (DirectX::SimpleMath::Vector3 vertex : (*mesh).Vertices) {

			DirectX::SimpleMath::Vector2 point = Project(vertex, transformMatrix);

			DrawPoint(point);
		}
	}
}
