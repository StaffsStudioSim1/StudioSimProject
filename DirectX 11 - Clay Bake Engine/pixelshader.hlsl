struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE: register(t0);
SamplerState objSamplerState : SAMPLER: register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 sampleColour = objTexture.Sample(objSamplerState, input.inTexCoord);
    return sampleColour;
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
}