#include "Mesh.h"

Mesh::Mesh()
{
	name = "";
	Vertices.resize(0);
	Faces.resize(0);
}

Mesh::Mesh(std::string name)
{
	this->name = name;

}
