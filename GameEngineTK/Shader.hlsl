//�O���[�o��
Texture2D g_Texture: register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
	matrix g_WVP; //���[���h����ˉe�܂ł̕ϊ��s��
};

//�\����
struct GS_INPUT
{
	float4 Pos : SV_POSITION;
};



struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS(float4 Pos : POSITION/*, float2 Tex : TEXCOORD*/)
{
	PS_INPUT Out = (PS_INPUT)0;
	//Out.Pos = Pos;//�����Œ��_��ϊ����܂���B�Ȃ��Ȃ烂�f�����W�̂܂܂ŃW�I���g���V�F�[�_�[�ɓn����������ł��B
	Out.Pos = mul(Pos, g_WVP);
	//Out.Tex = Tex;
	return Out;

}
//
//
//�W�I���g���V�F�[�_�[
[maxvertexcount(4)]
void GS_Point(point GS_INPUT Input[1], inout TriangleStream <PS_INPUT> TriStream)
{
	float w = 0;//�S�̒��_�S�Ă�w�l���P�Ԗ�[0]�̒��_��w�ɍ��킹��B���킹��͉̂��Ԗڂł������@�S�Ă������Ȃ�i�|���S���Ƀp�[�X���������Ă��܂��̂�h�~�j

	PS_INPUT p = (PS_INPUT)0;
	p.Pos = mul(Input[0].Pos, g_WVP);
	w = p.Pos.w;
	p.Tex = float2(0, 1);
	TriStream.Append(p);

	p.Pos = Input[0].Pos + float4(12, 0, 0, 0);
	p.Pos = mul(p.Pos, g_WVP);
	p.Pos.w = w;
	p.Tex = float2(0, 0);
	TriStream.Append(p);

	p.Pos = Input[0].Pos + float4(0, -12, 0, 0);
	p.Pos = mul(p.Pos, g_WVP);
	p.Pos.w = w;
	p.Tex = float2(1, 1);
	TriStream.Append(p);

	p.Pos = Input[0].Pos + float4(12, -12, 0, 0);
	p.Pos = mul(p.Pos, g_WVP);
	p.Pos.w = w;
	p.Tex = float2(1, 0);
	TriStream.Append(p);

	TriStream.RestartStrip();
}


//�s�N�Z���V�F�[�_�[
float4 PS(PS_INPUT Input) : SV_Target
{
	//return float4(1,1,1,1);
	return g_Texture.Sample(g_Sampler, Input.Tex);
}