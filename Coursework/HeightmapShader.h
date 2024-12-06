#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class HeightmapShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse;
		XMFLOAT3 direction;
		float padding;
	};

	struct HeightmapBufferType
	{
		float maxHeight;
		
	};

public:
	HeightmapShader(ID3D11Device* device, HWND hwnd);
	~HeightmapShader();
	
	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, float height);



private:
	void initShader(const wchar_t* cs, const wchar_t* ps);
	

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* heightmapBuffer;
};

