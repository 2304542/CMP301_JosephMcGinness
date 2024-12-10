// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "ManipulationShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "LightShader.h"
#include "HeightmapShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();
	void depthPass();
	void firstPass();
	void finalPass();

private:
	PlaneMesh* sea;
	PlaneMesh* sand;
	Light* light[2];
	ManipulationShader* manipulationShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;
	LightShader* lightShader;
	HeightmapShader* heightmapShader;
	CubeMesh* bunny;
	ShadowMap* shadowMap[2];
	ID3D11ShaderResourceView* shadowMapGetter[2];
	float time; // enables wave movement 

	float amplitude = 0.1f; // height of waves
	float frequency = 0.1f;
	float speed = 1.0f;

};

#endif