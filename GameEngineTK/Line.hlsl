//�O���[�o��
cbuffer global
{
	matrix g_WVP; //���[���h����ˉe�܂ł̕ϊ��s��
};
//�\����
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS(float4 Pos : POSITION)
{
	PS_INPUT Out;
	Out.Pos = mul(Pos, g_WVP);
	return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS(PS_INPUT Input) : SV_Target
{
	return float4(1,1,1,1);
}