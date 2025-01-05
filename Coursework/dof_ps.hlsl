Texture2D sceneTexture : register(t0);
Texture2D blurTexture : register(t1);

SamplerState Sampler0 : register(s0);


struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float weight = 0.5f;
    float4 sceneSample = sceneTexture.Sample(Sampler0, input.tex);
    float4 blurSample = blurTexture.Sample(Sampler0, input.tex);
	// Distance to the center of the screen
    float centerDist = sceneTexture.Sample(Sampler0, float2(0.5f, 0.5f)).w;

    //the difference in depth between this and the centre point
    float depthDifference = abs(centerDist - sceneSample.w) * weight;

	//Get a value based on the difference 
    return saturate(lerp(sceneSample, blurSample, depthDifference));

}