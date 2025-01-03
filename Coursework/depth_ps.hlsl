struct InputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD0;
};

cbuffer TimeBuffer : register(b1)
{
    float time;
    float amplitude = 0.1f;
    float speed;
    float frequency;
}

float4 main(InputType input) : SV_TARGET
{
    input.position.y = cos(input.position.z * frequency + (time * speed)) * amplitude + sin(input.position.x * frequency + (time * speed)) * amplitude;
    float depthValue;
	// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    depthValue = input.depthPosition.z / input.depthPosition.w;
    return float4(depthValue, depthValue, depthValue, 1.0f);

}