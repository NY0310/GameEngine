//�O���[�o��
cbuffer global
{
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};
//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mWVP);

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	return float4(0.5f, 0.5f, 0.5f, 1.0f);
}

//�e�N�j�b�N�͕s�v�B�@�ǂ̃V�F�[�_�[���g�����A�v�����Ŏw�肷��̂ŁB