// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"
#include "ManipulationShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "LightShader.h"

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
	void finalPass();

private:
	PlaneMesh* sea;
	PlaneMesh* sand;
	Light* light;
	ManipulationShader* manipulationShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;
	LightShader* lightShader;
	CubeMesh* bunny;
	ShadowMap* shadowMap;
	float time; // enables wave movement 

	float amplitude = 0.1f; // height of waves
	float frequency = 0.1f;
	float speed = 1.0f;

};

#endif
