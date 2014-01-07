#pragma once

#include "quad.h"
#include <vector>

class Mesh
{
public:
	void push_back(const Quad& q);
	void concatenate(const Mesh& m);
	void draw() const;

private:
	std::vector<Quad> _quads;
};