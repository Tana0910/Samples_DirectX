// GeometryShader.hlsl

cbuffer MatrixBuffer : register(b0)
{
	column_major matrix matproj;
	column_major matrix matview;
	column_major matrix matworld;
}

struct GS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL; // 面法線
};

[maxvertexcount(3)]
void main(triangle GS_INPUT input[3], inout TriangleStream<PS_INPUT> tristream)
{
	PS_INPUT output = (PS_INPUT)0;
	// 面法線の取得
	float3 edge1 = input[1].pos.xyz - input[0].pos.xyz;
	float3 edge2 = input[2].pos.xyz - input[0].pos.xyz;
	output.normal = normalize(cross(edge1, edge2));
	for (int i = 0; i < 3; ++i)
	{
		output.pos = mul(input[i].pos, matworld); // ローカル座標からワールド座標へ変換
		output.pos = mul(output.pos, matview);    // ワールド座標からビュー座標(カメラ座標)へ変換
		output.pos = mul(output.pos, matproj);    // ビュー座標からプロジェクション座標へ変換
		output.color = input[i].color;            // 色情報は入力値をそのまま渡す
		tristream.Append(output);                 // 三角形ポリゴン単位でデータを格納する
	}
	tristream.RestartStrip();
}
