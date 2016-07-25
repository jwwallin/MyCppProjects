#pragma once
#include "MyIncludes.h"
#include <string>

class Mesh
{
public:
	Mesh();
	Mesh(std::string name);

	std::string													name;
	std::vector<DirectX::SimpleMath::Vector3>					Vertices;
	DirectX::XMVECTOR											Position;
	DirectX::SimpleMath::Quaternion								Rotation;

};

