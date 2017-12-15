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
	float4 inkUv;//�C���N��UV
	float inkScale;//�C���N�e�N�X�`����UV���W

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
	//���K�f�o�C�X���W�n�ɕϊ���UV�l�ɑΉ�����
//	pos.xyz /= (pos.w + 1) * 0.2f;
	
	return output;
}


/// <summary>
/// �s�N�Z���V�F�[�_�[
/// </summary>
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color;
	if (inkUv.x - inkScale < input.Tex.x && input.Tex.x < inkUv.x + inkScale &&
		inkUv.y - inkScale < input.Tex.y &&
		input.Tex.y < inkUv.y + inkScale)
	{
		color = g_texInk.Sample(g_samLinear, (inkUv.xy - input.Tex) / inkScale * 0.5f + 0.5f);
		if (color.a - 1 >= 0)
		{
			color = inkColor;
		}
		else
		{
			discard;
		}
	
	}
	else
	{
		discard;
	}
	return color;
}











