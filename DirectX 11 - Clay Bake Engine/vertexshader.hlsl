Texture2D objTexture : TEXTURE: register(t0);
SamplerState sam : register (s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 InTex : TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 PosW : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input) //: SV_POSITION
{

    VS_OUTPUT output = (VS_OUTPUT)0;
    output.PosW = mul(float4(input.Pos, 1.0f), World);
    //output.PosW = pos;
    //output.Tex = (sam, input.InTex);
    //output.Tex = tex;

    return output;
}