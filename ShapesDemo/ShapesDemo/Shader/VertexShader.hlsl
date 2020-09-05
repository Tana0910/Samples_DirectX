// VertexShader.hlsl
// 頂点データを描画するための処理を記述する

// 定数バッファ(constant buffer). スロット番号0にバッファを割り当てる.
cbuffer MatrixBuffer : register(b0)
{
	// 列優先(column_major)として行列を定義する
	column_major matrix matproj;
	column_major matrix matview;
	column_major matrix matworld;
};

// プログラムから受け取る頂点データの定義
struct VS_INPUT
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

// ジオメトリシェーダーに渡すデータの定義
struct GS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

// エントリーポイント
// 返り値はGS_INPUT
GS_INPUT main(VS_INPUT input)
{
	GS_INPUT output = (GS_INPUT)0;
	output.pos = float4(input.pos, 1); // w座標値を追加
	output.color = input.color;
	return output;
}
