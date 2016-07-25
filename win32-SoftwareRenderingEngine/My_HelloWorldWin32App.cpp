#include "My_HelloWorldWin32App.h"
#include "Window.h"

bool Run(Window*);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	uint32_t size_x = 1024, size_y = 768;

	COLORREF * buffer = new COLORREF[size_x*size_y];


	Window * window;
	window = new Window(size_x, size_y, "my window", buffer);

	Device dvc = Device(buffer, size_x, size_y);

	Mesh mesh = Mesh("Cube");
	Camera camera = Camera();

	camera.Position = DirectX::SimpleMath::Vector3(0, 0, 10.0f);
	camera.Target = DirectX::SimpleMath::Vector3(0, 0, 0);

	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(-1, 1, 1));
	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(1, 1, 1));
	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(-1, -1, 1));
	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(-1, -1, -1));
	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(-1, 1, -1));
	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(1, 1, -1));
	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(1, -1, 1));
	mesh.Vertices.push_back(DirectX::SimpleMath::Vector3(1, -1, -1));

	std::vector<Mesh*> meshes;
	meshes.push_back(&mesh);

	while (Run(window)) {
		dvc.Clear(0, 0, 0);

		dvc.DrawPoint(DirectX::SimpleMath::Vector2(0,0));

		dvc.Render(&camera, meshes);

		mesh.Rotation = mesh.Rotation * DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(0.01, 0, 0);

		window->Update();
	}

	free(buffer);
	free(window);

	return 0;
}

bool Run(Window* window) {
	if (window != NULL) {
		return window->Update();
	}

	return true;
}