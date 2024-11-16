#pragma once

#include "DXF.h"

using namespace DirectX;

class Terrain
{
private:
	void LoadHeightmap();
	float Average(int i, int j);
};