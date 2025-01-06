#pragma once


#include "DXF.h"
#include "DoFBuffer.h"


class ShaderFunctions
{
	
	public:
		//Sets buffer description up, saves code repetition
		static void CreateBufferDesc(std::size_t size, ID3D11Buffer** buffer, ID3D11Device* renderer);
		//Don't use these unless they definitely match the sampler descs already set up
		static void CreateSamplerDesc(ID3D11SamplerState** sampler, ID3D11Device* renderer);
		static void CreateTextureSamplerDesc(ID3D11SamplerState** sampler, ID3D11Device* renderer);
};


