Texture2D unblurredSceneTexture : register(t0);
Texture2D blurSceneTexture : register(t1);
Texture2D depthSceneTexture : register(t2);

SamplerState SampleType : register(s0);

cbuffer DepthBuffer : register(b0)
{
    float range;
    float nearValue;
    float farValue;
    float offset;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 colour = float4(0, 0, 0, 1);
   
    float4 unblurredScene = unblurredSceneTexture.Sample(SampleType, input.tex);
    float4 blurScene = blurSceneTexture.Sample(SampleType, input.tex);
    float depthvalue = depthSceneTexture.Sample(SampleType, input.tex).r;
    float centreTexel = depthSceneTexture.Sample(SampleType, float2(0.5f, 0.5f)).r;


    centreTexel *= (nearValue - farValue);
    depthvalue *= (nearValue - farValue);

    float s = saturate(abs(depthvalue - centreTexel + offset) / range);
      

    colour = lerp(unblurredScene, blurScene, s);
    return colour;

}