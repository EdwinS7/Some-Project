cbuffer vertexBuffer : register(b0)
{
    float4x4 ProjectionMatrix;
};

struct VS_INPUT
{
    float4 pos : POSITION; // x, y, z, rhw
    float2 uv : TEXCOORD0; // u, v
    float4 col : COLOR0; // r, g, b, a
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
        
    output.pos = mul(ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));
    output.col = input.col;
    output.uv = input.uv;

    return output;
}