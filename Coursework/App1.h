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
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "TextureShader.h"
#include "DoFShader.h"

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
	
	
	void verticalBlur();
	void horizontalBlur();
	void DepthOfFieldShaderPass();
	void depthPass();
	void finalPass();

private:
	PlaneMesh* sea;
	PlaneMesh* sand;
	OrthoMesh* orthoMesh;
	Light* light[2];

	AModel* model;



	XMFLOAT3 lightPos1;
	XMFLOAT3 lightDir1;

	XMFLOAT3 lightPos2;
	XMFLOAT3 lightDir2;

	CubeMesh* cube;
	SphereMesh* sphere;

	XMFLOAT3 cubePos;


	ManipulationShader* manipulationShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;
	LightShader* lightShader;
	HeightmapShader* heightmapShader;
	HorizontalBlurShader* hBlurShader;
	VerticalBlurShader* vBlurShader;
	TextureShader* textureShader;
	DepthOfFieldShader* depthOfFieldShader;

	RenderTexture* renderTexture;
	RenderTexture* hBlurTexture;
	RenderTexture* vBlurTexture;
	RenderTexture* depthOfFieldTexture;

	CubeMesh* bunny;
	ShadowMap* shadowMap[2];
	ID3D11ShaderResourceView* shadowMapGetter[2];
	float time; // enables wave movement 

	float amplitude = 0.1f; // height of waves
	float frequency = 0.1f;
	float speed = 1.0f;

	float dofDistance = 1.0f;
	float dofOffset = 0.0f;
	float vStrength = 1.0;
	float hStrength = 1.0;

};

#endif