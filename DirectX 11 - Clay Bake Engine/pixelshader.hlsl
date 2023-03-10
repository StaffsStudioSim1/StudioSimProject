cbuffer ObjectBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4x4 TexMatrix;
    float AlphaMultiplier;
}

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE: register(t0);
SamplerState samplerState : SAMPLER: register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 sampleColour = objTexture.Sample(samplerState, input.inTexCoord);
    sampleColour.a *= AlphaMultiplier;
    if (sampleColour.a < 0.1f)
        discard;
    
    return saturate(sampleColour);
}