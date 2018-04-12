//cbuffer global
//{
//	matrix wvp;
//	float4 color;
//};
//
//struct vsOutput 
//{
//	float4 Pos : SV_POSITION;
//};
//
//
///// <summary>
///// �o�[�e�b�N�X�V�F�[�_�[
///// </summary>
//vsOutput VS(float4 Pos : POSITION)
//{
//	vsOutput output = (vsOutput)0;
//	output.Pos = mul(Pos, wvp);
//	return output;
//}
//
//
//float4 PS(vsOutput input) : SV_TARGET
//{
//	return color;
//}



//�O���[�o��
cbuffer global_0:register(b0)
{
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 color;
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD1;
	float2 Tex : TEXCOORD3;
};






//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	//�@�������[���h��Ԃ�
	output.Pos = mul(Pos, g_mWVP);


	return output;
}




//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	return color;
}
