// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);


SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambientColour;
    float4 diffuseColour;
    float3 lightDirection;
    float padding;
    float3 lightPosition;
    float padding2;
    float range;
};

cbuffer TimeBuffer : register(b1)
{
    float time;
    float amplitude = 0.1f;
    float speed;
    float frequency;
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float getDisplacement(float2 uv)
{
    float offset = texture0.SampleLevel(sampler0, uv, 0).r;
    return offset;
}


float3 CalculateNormals(float2 uv)
{
   
    float2 dimensions;
    texture0.GetDimensions(dimensions.x, dimensions.y);
    float uvOff = amplitude / min(dimensions.x, dimensions.y);
    float hTop = getDisplacement(float2(uv.x, uv.y + uvOff));
    float hBottom = getDisplacement(float2(uv.x, uv.y - uvOff));
    float hRight = getDisplacement(float2(uv.x + uvOff, uv.y));
    float hLeft = getDisplacement(float2(uv.x - uvOff, uv.y));
    
    float step = (100.0f * amplitude)  * uvOff;
    float3 tan = normalize(float3(2.0f * step, hRight - hLeft, 0));
    float3 bitan = normalize(float3(0, hTop - hBottom, 2.0f * step));
    return cross(bitan, tan);

}

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = ambientColour + saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour;
   
    input.position.y = cos(input.position.z * frequency + (time * speed)) * amplitude + sin(input.position.x * frequency + (time * speed)) * amplitude;
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);
    input.normal = CalculateNormals(input.tex);
    input.normal = normalize(input.normal);
    lightColour = ambientColour + calculateLighting(-lightDirection, input.normal, diffuseColour);
	
    return lightColour * textureColour;

}