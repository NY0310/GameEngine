//�O���[�o��
Texture2D g_texColor: register(t0);
//exture2D g_texInk: register(t1);

SamplerState g_samLinear : register(s0);
//SamplerState g_InkLinear : register(s2);

//�O���[�o��
cbuffer global_0:register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir;  //���C�g�̕����x�N�g��
	float4 g_vEye;//�J�����ʒu
	matrix g_mWPVT;
	//bool g_ObjTexFlag;//�I�u�W�F�N�g�̃e�N�X�`����		

};

cbuffer global_1:register(b1)
{
	float4 g_Ambient = float4(0, 0, 0, 0);//�A���r�G���g��
	float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
	float4 g_Specular = float4(1, 1, 1, 1);//���ʔ���

};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float4 Tex : TEXCOORD3;
};
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

	////�e�N�X�`���[���W
	//if (g_ObjTexFlag)
	//{
	//	output.Tex = Tex;
	//}
	//else
	//{
	output.Tex = mul(Tex, g_mWPVT);
	//}
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
	input.Tex.xyz /= input.Tex.z;
	//color = g_texInk.Sample(g_samLinear, input.Tex);
	color = g_texColor.Sample(g_samLinear, input.Tex);
	//�e�N�X�`�������݂���ꍇ�͂��̐F��D�悷��
	//color += input.Color / 2;	

	return color;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	float4 color = input.Color;

	return color;
}