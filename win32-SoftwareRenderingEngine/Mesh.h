#pragma once
#include "MyIncludes.h"
#include <string>

struct Face {
public:
	Face() {};
	Face(int A, int B, int C) { this->A = A; this->B = B; this->C = C; };
	int A;
	int B;
	int C;
};

class Mesh
{
public:
	Mesh();
	Mesh(std::string name);

	std::string													name;
	std::vector<DirectX::SimpleMath::Vector3>					Vertices;
	std::vector<Face>											Faces;
	DirectX::XMVECTOR											Position;
	DirectX::SimpleMath::Quaternion								Rotation;

};

