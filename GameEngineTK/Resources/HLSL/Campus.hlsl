/// <summary>
///	�I�u�W�F�N�g�ɓ\��C���N���e�N�X�`�����쐬
/// </summary>

//�O���[�o��
Texture2D texNewInk: register(t0);//�C���N�e�N�X�`��
Texture2D texCampus: register(t1);//�C���N�e�N�X�`��
texture2D normalMap: register(t2);//�m�[�}���}�b�v

SamplerState samLinear : register(s0);


cbuffer global_0:register(b0)
{
	float4 inkColor;//�C���N�̊�{�F
	//float inkScale;//�C���N�̑傫��
};


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};





/// <summary>
/// �C���N��h�邩
/// </summary>
bool IsPaint(float2 inkuv, float2 inputuv, float scale)
{
	return (inkuv.x - scale < inputuv.x && inputuv.x < inkuv.x + scale &&
		inkuv.y - scale < inputuv.y &&
		inputuv.y < inkuv.y + scale);

}




/// <summary>
/// �������ǂ���
/// </summary>
bool IsOpacity(float color) {
	return (!color <= 0);
}


/// <summary>
/// ��F���u�����h����
/// </summary>
float4 Blend(float4 color, float4 color1, float brushAlpha) {
	return color * (1 - brushAlpha * color.a) + color1 * color1.a * brushAlpha;
}


/// <summary>
/// ���K�f�o�C�X���W�n����Uv�l�֕ϊ�����
/// </summary>
float2 RegularDeviceChangeUv(float2 uv)
{
	return float2((uv.x + 1) / 2, (uv.y + 1) / 2);
}


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

	//�V�K�C���N�̃J���[
	float4 newColor = texNewInk.Sample(samLinear,input.Tex);
	//�L�����p�X�J���[
	float4 campusColor = texCampus.Sample(samLinear, clamp(RegularDeviceChangeUv(input.Pos.xy),0,1));
	//�V�K�C���N���s������
	const bool IsOpacitytNewColor = IsOpacity(newColor.a);
	//�L�����p�X���s������
	const bool IsOpacityCampusColor = IsOpacity(campusColor.a);


	//�V�K�C���N�̃J���[�L�����p�X�J���[���ɓ����̏ꍇ�o�͂Ȃ�
	if (!IsOpacitytNewColor&& !IsOpacityCampusColor)discard;	

	

	//�V�K�C���N���s������
	if (IsOpacitytNewColor)
	{
		//�m�[�}���}�b�v���獂������ǂݎ��
		float4 hight = normalMap.Sample(samLinear, input.Tex);
		newColor.a = 0.299 * hight.x + 0.587 * hight.y + 0.114 * hight.z;
		//�C���N�̐F���i�[
		newColor.xyz = inkColor.xyz;
	}

	//�V�K�C���N�̃J���[�L�����p�X�J���[���ɕs�����̏ꍇ�F���u�����h�������𑫂��o��
	if (IsOpacitytNewColor && IsOpacityCampusColor)
		//return float4((newColor.xyz + campusColor.xyz) / 2, newColor.a + campusColor.a);
		return newColor;

	//�V�K�C���N�̃J���[�̂ݕs�����̏ꍇ�V�K�C���N�̏��̂ݏo��
	if (IsOpacitytNewColor && !IsOpacityCampusColor)
		return newColor;

	//�L�����p�X�J���[�̂ݏo��
	return campusColor;

}


