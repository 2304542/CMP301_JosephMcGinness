
cbuffer MatrixBuffer : register(b0)
{

    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer : register(b1)
{
    float time;
    float amplitude;
    float speed;
    float frequency;
}

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
/*float3 CalculateNormals(float2 uv) {
    float offset = 1.0f / 1000.0f;
    float hRight = HeightMap.SampleLevel(sampler0, uv + float2(0.001, 0.0), 0);
    float hLeft = HeightMap.SampleLevel(sampler0, uv - float2(0.001, 0.0), 0);
    float hTop = HeightMap.SampleLevel(sampler0, uv + float2(0.0, 0.001), 0);
    float hBottom = HeightMap.SampleLevel(sampler0, uv - float2(0.0, 0.001), 0);

    float step = 100.0f * diff;
    float3 tan = normalize(float3(2.0f * step, hRight - hLeft, 0));
    float3 bitan = normalize(float3(0, hTop - hBottom, 2.0f * step));

    return cross(bitan, tan);

} */
OutputType main(InputType input)
{
    OutputType output;

	//offset position based on sine wave
    input.position.y = cos(input.position.z * frequency + (time * speed)) * amplitude + sin(input.position.x * frequency + (time * speed)) * amplitude;

	//modify normals
    input.normal = float3(-cos(input.position.x + time), 1, 0);

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