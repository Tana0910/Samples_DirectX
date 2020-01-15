// VertexShader.hlsl
// ���_�f�[�^��`�悷�邽�߂̏������L�q����

// �萔�o�b�t�@(constant buffer). �X���b�g�ԍ�0�Ƀo�b�t�@�����蓖�Ă�.
cbuffer MatrixBuffer : register(b0)
{
	// ��D��(column_major)�Ƃ��čs����`����
	column_major matrix matproj;
	column_major matrix matview;
	column_major matrix matworld;
};

// �v���O��������󂯎�钸�_�f�[�^�̒�`
struct VS_INPUT
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

// �W�I���g���V�F�[�_�[�ɓn���f�[�^�̒�`
struct GS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

// �G���g���[�|�C���g
// �Ԃ�l��GS_INPUT
GS_INPUT main(VS_INPUT input)
{
	GS_INPUT output = (GS_INPUT)0;
	output.pos = float4(input.pos, 1); // w���W�l��ǉ�
	output.color = input.color;
	return output;
}
