/// <summary>
///	�I�u�W�F�N�g�ɓ\��C���N���e�N�X�`�����쐬
/// </summary>

//�O���[�o��
Texture2D g_texInk: register(t0);//�C���N�e�N�X�`��

SamplerState g_samLinear : register(s0);
//�O���[�o��



cbuffer global_0:register(b0)
{
	float4 inkColor;//�C���N�̊�{�F
};


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


/// <summary>
/// �o�[�e�b�N�X�V�F�[�_�[
/// </summary>
VS_OUTPUT VS(float4 Pos : POSITION ,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Tex = Tex;
	output.Pos =  Pos;	
	return output;
}


/// <summary>
/// �s�N�Z���V�F�[�_�[
/// </summary>
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color;
	color = g_texInk.Sample(g_samLinear,input.Tex);
	if (color.a - 1 >= 0)
	{
		color = inkColor;
	}
	else
	{
		discard;
	}
	

	return color;
}











