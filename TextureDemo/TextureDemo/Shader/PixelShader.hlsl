// PixelShader.hlsl
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

Texture2D myTexture : register(t0);
SamplerState mySampler : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//return float4(input.color.xyz, input.color.w);
	return myTexture.Sample(mySampler, input.tex);
}
