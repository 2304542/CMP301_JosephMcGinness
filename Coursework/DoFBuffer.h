#pragma once
#include "DXF.h"

struct MatrixBufferType
{


	XMMATRIX lightViewMatrix1;
	XMMATRIX lightProjectionMatrix1;

	XMMATRIX lightViewMatrix2;
	XMMATRIX lightProjectionMatrix2;


};

struct LightBufferType
{
	XMFLOAT4 ambient1;
	XMFLOAT4 ambient2;
	XMFLOAT4 ambient3;

	XMFLOAT4 diffuse1;
	XMFLOAT4 diffuse2;
	XMFLOAT4 diffuse3;

	XMFLOAT4 lightDirection1;
	XMFLOAT4 lightDirection2;
	XMFLOAT4 lightDirection3;
};
