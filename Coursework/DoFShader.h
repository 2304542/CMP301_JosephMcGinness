#pragma once

#include "DXF.h"


using namespace std;
using namespace DirectX;

class DepthOfFieldShader : public BaseShader
{
	struct DepthBufferType
	{
		float range;
		float nearValue;
		float farValue;
		float offsetValue;
	};

public:

	DepthOfFieldShader(ID3D11Device* device, HWND hwnd);
	~DepthOfFieldShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* sceneTexture, ID3D11ShaderResourceView* blurTexture, float nearVal, float farVal, float range, float offset);

private:
	void initShader(const wchar_t* cs, const wchar_t* ps);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
};