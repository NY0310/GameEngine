cbuffer GLOBAL
{
	matrix g_mW: packoffset(c0);//c(n) n��16�o�C�g�P��
	matrix g_mWVP: packoffset(c4);
	matrix g_mWLPT: packoffset(c8);
	matrix g_mWLP: packoffset(c12);
	float4 g_vLightPos: packoffset(c16);//���C�g�̕����x�N�g��
	float4 g_Ambient: packoffset(c17);//�A���r�G���g��
	float4 g_Diffuse: packoffset(c18); //�g�U����
	float4 g_Specular: packoffset(c19);//���ʔ���
	float3 g_vEye: packoffset(c20);
	float g_Transparent : packoffset(c21);//�ǂ��܂ł̒����Ȃ瓧�߂��邩
};

Texture2D g_texDepth;
SamplerState g_samLinear;
//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUT
{
	float4 Pos : SV_POSITION;
};
//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float LightOut : LIGHT_OUT;
	float4 texcoord :TEXCOORD3;
};

//
//�[�x�e�N�X�`���쐬�@�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUT VS_Depth(float4 Pos : POSITION)
{
	VS_OUT output = (VS_OUT)0;
	output.Pos = mul(Pos, g_mWLP);

	return output;
}

//
//�[�x�e�N�X�`���쐬�@�s�N�Z���V�F�[�_�[
//
float4 PS_Depth(VS_OUT input) : SV_Target
{
	return float4(input.Pos.w,0,0,1);//R32�t�H�[�}�b�g�Ȃ̂ŐԈȊO�͕s�g�p
}

//
//SSS�@�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Normal, (float3x3)g_mWVP);
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;
	output.Light = normalize(g_vLightPos - PosWorld);

	float4 texcoord = mul(Pos, g_mWLPT);	//���̓_���A���C�g�r���[�ł������Ƃ��̈ʒu���킩��
	output.texcoord = texcoord;
	float4 Plight = mul(Pos, g_mWLP);
	float LightOut = Plight.w;//���C�g�o��
	output.LightOut = LightOut;

	return output;
}

//
//SSS�@�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = float4(1,0,0,1);
	//�t�H���V�F�[�f�B���O
	float3 Normal = normalize(input.Normal);
	float3 LightVector = normalize(input.Light);
	float3 EyeVector = normalize(input.EyeVector);
	float NdotL = dot(Normal, LightVector);
	float3 Reflect = normalize(2 * NdotL*Normal - LightVector);
	float specular = pow(saturate(dot(Reflect, EyeVector)), 8);
	float4 phong = g_Diffuse * saturate(NdotL) + 4 * specular*g_Specular;
	//���b�V���A���Y�n�_�̌��݂����߂�
	float LightOut = input.LightOut;
	float LightIn = 0;
	input.texcoord.xyz /= input.texcoord.w;
	LightIn = g_texDepth.Sample(g_samLinear, input.texcoord).r;
	float thickness = abs(LightOut - LightIn);
	//���߂����	
	float4 transparent = g_Diffuse*(1.0f - saturate(thickness / g_Transparent));
	
	color = phong  * 4 + 2 * transparent*3.0f / 4.0f;//�t�H���V�F�[�f�B���O�̐F�ɓ��ߌ����v���X

	return color;
}