SamplerState sampler0 : register(s0);

Texture2D HeightMap : register(t0);

cbuffer MatrixBuffer : register(b0)
{

    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer HeightmapBuffer : register(b1)
{
    float maxHeight;
    float3 padding;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float getDisplacement(float2 uv)
{
    float offset = HeightMap.SampleLevel(sampler0, uv, 0).r;
    return offset * maxHeight;
}


OutputType main(InputType input)
{
    OutputType output;

    input.position.y = getDisplacement(input.tex);
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    return output;
}