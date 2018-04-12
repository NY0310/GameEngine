Texture2D texColor: register(t0);//�V�F�[�f�B���O���郂�f���e�N�X�`��
Texture2D texInk: register(t1);//�C���N�e�N�X�`��
Texture2D texDepth: register(t2);//�[�x�e�N�X�`��

//�T���v���[
SamplerState samLinear : register(s0);


//�O���[�o��
cbuffer global_0:register(b0)
{
	matrix mW;//���[���h�s��
	matrix mWVP; //���[���h�E�r���[�E�v���W�F�N�V�����s��
	matrix mWLP; //���[���h�E���C�g�r���[�E�v���W�F�N�V�����s��
	matrix mWLPT; //���[���h�E���C�g�r���[�E�v���W�F�N�V�����EUV�s��
	float4 vLightDir;  //���C�g�̎��_
	float4 vEye;//�J�����ʒu
};

cbuffer global_1:register(b1)
{
	float4 ambient = float4(0, 0, 0, 1.0f);//�A���r�G���g��
	float4 diffuse = float4(0.78f, 0.78f, 0.78f, 1); //�g�U����(�F�j
	float4 specular = float4(0.4f, 0.4f, 0.4f, 1.0f);//���ʔ���

};


//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float4 PosWorld : TEXCOORD3;
	float2 Tex : TEXCOORD4;
	float4 LightTexCoord : TEXCOORD5;
	float4 LighViewPos : TEXCOORD56;
};






//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�ˉe�ϊ�
	output.Pos = mul(Pos, mWVP);
	//�@�������f���̉�]�ɍ��킹��
	output.Normal = (float3)mul((float3)Norm, (float3x3)mW);
	//���C�g����
	output.Light = normalize((float3)vLightDir);
	//���[���h���W
	output.PosWorld = mul(Pos, mW);
	//�����x�N�g��	
	output.EyeVector = normalize((float3)(vEye - output.PosWorld));
	//UV�l
	output.Tex = Tex;
	//���C�g�r���[���Q�Ƃ���Ƃ��A�肪����ƂȂ�e�N�X�`�����W
	output.LightTexCoord = mul(Pos, mWLPT);
	//���C�g�r���[�̍��W
	output.LighViewPos = mul(Pos, mWLP);

	return output;
}


//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS_NoTex(float4 Pos : POSITION, float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�ˉe�ϊ�
	output.Pos = mul(Pos, mWVP);
	//�@�������f���̉�]�ɍ��킹��
	output.Normal = (float3)mul((float3)Norm, (float3x3)mW);
	//���C�g����
	output.Light = (float3)normalize(vLightDir);
	//���[���h���W
	output.PosWorld = mul(Pos, mW);
	//�����x�N�g��	
	output.EyeVector = (float3)normalize(vEye - output.PosWorld);
	//���C�g�r���[���Q�Ƃ���Ƃ��A�肪����ƂȂ�e�N�X�`�����W
	output.LightTexCoord = mul(Pos, mWLPT);
	//���C�g�r���[�̍��W
	output.LighViewPos = mul(Pos, mWLP);


	return output;
}


/// <summary>
/// �������ǂ���
/// </summary>
bool IsOpacity(float color) {
	return (!color <= 0);
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{

	//�L�����p�X�̐F
	float4 campusColor = texInk.Sample(samLinear, input.Tex);
	//���f���̐F
	float4 modelColor = texColor.Sample(samLinear, input.Tex);



	///// <summary>
	///// �V���h�E�}�b�v
	///// </summary>
	//input.LightTexCoord /= input.LightTexCoord.w;
	//float TexValue = texDepth.Sample(samLinear,input.LightTexCoord.xy).r;
	//float LightLength = input.LighViewPos.z / input.LighViewPos.w;
	////���C�g�r���[�ł̒������Z���̂ŏ�Q������
	//if (TexValue + 0.001<LightLength)
	//{
	//	//���f���ƃL�����p�X�̐F�Ɍ���Ɖe��K��������
	//	modelColor.rgb /= 3;
	//	campusColor.rgb /= 3;
	//}


	//�V�K�C���N���s������
	const bool IsOpacityModelColor = IsOpacity(modelColor.a);
	//�L�����p�X���s������
	const bool IsOpacityCampusColor = IsOpacity(campusColor.a);



	if (!IsOpacityModelColor && !IsOpacityCampusColor)
		discard;

	if (IsOpacityModelColor && !IsOpacityCampusColor)
	{
		/// <summary>
		/// �t�H���V�F�[�f�B���O
		/// </summary>
		////�����@
		//float NL = saturate(dot(input.Normal, input.Light));
		////�g�U���ˌ��@
		//float4 diffuse_ = (diffuse * 0.1 + modelColor * 0.9)*NL;
		////���ʔ��ˌ��@
		//float3 reflect = normalize(2 * NL*input.Normal - input.Light);
		//float4 specular_ = pow(saturate(dot(reflect, input.EyeVector)), 2)*specular;
		////�t�H�����f���ŏI�F
		//return  ambient + diffuse_ + specular_;
		return modelColor;
	}


	return campusColor;

}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//float4 color = input.Color;

	return float4(1,1,1,1);
}