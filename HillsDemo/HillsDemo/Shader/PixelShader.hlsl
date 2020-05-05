// PixelShader.hlsl
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(input.color.xyz, input.color.w);
}
