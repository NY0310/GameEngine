//�O���[�o��
Texture2D g_texColor: register(t0);//�V�F�[�f�B���O���郂�f���e�N�X�`��
Texture2D g_texInk: register(t1);//�C���N�e�N�X�`��
Texture2D g_texDepth: register(t2);//�[�x�e�N�X�`��

SamplerState g_samLinear : register(s0);


//�O���[�o��
cbuffer global_0:register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	matrix g_mWLP; //���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����̍���
	matrix g_mWLPT; //���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����EUV�s��̍���
	float4 g_vLightDir;  //���C�g�̕����x�N�g��
	float4 g_vEye;//�J�����ʒu
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient = float4(0, 0, 0, 0);//�A���r�G���g��
	float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
	float4 g_Specular = float4(1, 1, 1, 1);//���ʔ���

};

//�[�x�e�N�X�`���p�̃O���[�o��
cbuffer ZTexglobal:register(b2)
{
	matrix mWLP; //���[���h�E�h���C�g�r���[�h�E�v���W�F�N�V�����̍���
};
//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
	float4 LightTexCoord : TEXCOORD4;
	float4 LighViewPos : TEXCOORD5;
	//	float4 LTexCoord : TEXCOORD4;

};


//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT_DEPTH//�[�x�e�N�X�`���[�����_�[�p
{
	float4 Pos : SV_POSITION;
	float4 Depth :POSITION;
};



/// <summary>
/// �[�x�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
/// </summary>
VS_OUTPUT_DEPTH VS_Depth(float4 Pos : POSITION)
{
	VS_OUTPUT_DEPTH output = (VS_OUTPUT_DEPTH)0;
	output.Pos = mul(Pos, mWLP);
	return  output;
}


/// <summary>
/// �[�x�e�N�X�`���p�s�N�Z���V�F�[�_
/// </summary>
float4 PS_Depth(VS_OUTPUT_DEPTH input) :SV_Target
{
	float4 color = input.Depth.z / input.Depth.w;
	return color;
}

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	//�@�������[���h��Ԃ�
	output.Normal = mul(Norm, (float3x3)g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//���C�g����
	output.Light = g_vLightDir;
	//�����x�N�g��
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_Diffuse * NL + specular*g_Specular;

	output.Tex = Tex;

	//���C�g�r���[���Q�Ƃ���Ƃ��A�肪����ƂȂ�e�N�X�`�����W
	output.LightTexCoord = mul(Pos, g_mWLPT);
	//���C�g�r���[�̍��W
	output.LighViewPos = mul(Pos, g_mWLP);


	return output;
}


//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS_NoTex(float4 Pos : POSITION, float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	//�@�������[���h��Ԃ�
	output.Normal = mul(Norm, (float3x3)g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//���C�g����
	output.Light = g_vLightDir;
	//�����x�N�g��
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_Diffuse * NL + specular*g_Specular;


	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color;
	color = g_texInk.Sample(g_samLinear, input.Tex);
	/*if (color.a - 1 >= 0)
		color = g_texColor.Sample(g_samLinear, input.Tex);*/
	return color;
////�e�̏���
//input.LightTexCoord /= input.LightTexCoord.w;
//float TexValue = g_texDepth.Sample(g_samLinear, input.LightTexCoord).r;
//float LightLength = input.LighViewPos.z / input.LighViewPos.w;
//if (TexValue + 0.0001<LightLength)//���C�g�r���[�ł̒������Z���i���C�g�r���[�ł͎Օ���������j
//{
//	color /= 3;//�e
//}

}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	float4 color = input.Color;

	return color;
}