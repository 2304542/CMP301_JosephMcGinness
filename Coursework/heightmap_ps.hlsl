// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
Texture2D HeightMap : register(t1);
Texture2D depthMapTexture[2] : register(t2);
SamplerState sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
    float4 ambientColour;
    float4 diffuseColour;
    float3 lightDirection[2];
    float padding;
    float3 lightPosition;
    float padding2;
    float range;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPos : TEXCOORD1;
};

float getDisplacement(float2 uv)
{
    float offset = HeightMap.SampleLevel(sampler0, uv, 0).r;
    return offset * 100.0f;
}


float3 CalculateNormals(float2 uv)
{
   
    float2 dimensions;
    HeightMap.GetDimensions(dimensions.x, dimensions.y);
    float uvOff = 1.0f / min(dimensions.x, dimensions.y);
    float hTop = getDisplacement(float2(uv.x, uv.y + uvOff));
    float hBottom = getDisplacement(float2(uv.x, uv.y - uvOff));
    float hRight = getDisplacement(float2(uv.x + uvOff, uv.y));
    float hLeft = getDisplacement(float2(uv.x - uvOff, uv.y));
    
    float step = 100.0f * uvOff;
    float3 tan = normalize(float3(1.0f * step, (hRight - hLeft) / 2, 0));
    float3 bitan = normalize(float3(0, (hTop - hBottom) / 2, 1.0f * step));
    return cross(bitan, tan);

}

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = ambientColour + saturate(diffuse * intensity);
    return colour;
}

bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

bool isInShadow(Texture2D sMap, float2 uv, float4 lightViewPosition, float bias)
{
    // Sample the shadow map (get depth of geometry)
    float depthValue = sMap.Sample(shadowSampler, uv).r;
	// Calculate the depth from the light.
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        return false;
    }
    return true;
}

float2 getProjectiveCoords(float4 lightViewPosition)
{
    // Calculate the projected texture coordinates.
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}

float4 main(InputType input) : SV_TARGET
{
    float shadowMapBias = 10.00f;
    float4 textureColour;
    float4 lightColour;
   
    for (int i = 0; i < 2; i++)
    {
        float2 pTexCoord = getProjectiveCoords(input.lightViewPos);
    // Shadow test. Is or isn't in shadow
        if (hasDepthData(pTexCoord))
        {
        // Has depth map data
            if (!isInShadow(depthMapTexture[i], pTexCoord, input.lightViewPos, shadowMapBias))
            {
            // is NOT in shadow, therefore light
                lightColour = calculateLighting(-lightDirection[i], input.normal, diffuseColour[i]);
            }
        }
        lightColour = saturate(lightColour + ambientColour);
    }

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);
    input.normal = CalculateNormals(input.tex);
    input.normal = normalize(input.normal);
  
	
    return lightColour * textureColour;

}