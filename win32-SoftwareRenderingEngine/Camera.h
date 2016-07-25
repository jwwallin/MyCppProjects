#pragma once
#include "MyIncludes.h"
class Camera
{
public:
	Camera();
	~Camera();

	DirectX::SimpleMath::Vector3		Position;
	DirectX::SimpleMath::Vector3		Target;
};

