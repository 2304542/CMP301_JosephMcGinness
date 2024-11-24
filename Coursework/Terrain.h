#pragma once

#include "DXF.h"

using namespace DirectX;

class Terrain
{
private:
    struct HeightMapInfo {        // Heightmap structure
        int terrainWidth;        // Width of heightmap
        int terrainHeight;        // Height (Length) of heightmap
        XMFLOAT3* heightMap;    // Array to store terrain's vertex positions
    };
    int NumFaces = 0;
    int NumVertices = 0;
    void LoadHeightmap(char* filename, HeightMapInfo& hminfo);
};