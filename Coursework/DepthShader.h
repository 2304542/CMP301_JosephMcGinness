#p#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

struct TimeBufferType
{
	float time;
	float amplitude;
	float speed;
	float frequency;
};

class DepthShader : public BaseShader
{

public:

	DepthShader(ID3D11Device* device, HWND hwnd);
	~DepthShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, float time, float amplitude, float speed, float frequency);

private:
	void initShader(const wchar_t* vs, const wchar_t* ps);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* timeBuffer;
};