//�[�x�e�N�X�`���p�̃O���[�o��
cbuffer ZTexglobal:register(b0)
{
	matrix wlp; //���[���h�E���C�g�r���[�E�v���W�F�N�V�����s��
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VsOutput//�[�x�e�N�X�`���[�����_�[�p
{
	float4 pos : SV_POSITION;
	float4 depth :POSITION;
};


/// /// <summary>
/// �[�x�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
/// </summary>
VsOutput VS_Depth(float4 pos : POSITION)
{
	VsOutput output = (VsOutput)0;
	output.pos = mul(pos, wlp);
	output.depth = output.pos;
	return  output;
}


/// <summary>
/// �[�x�e�N�X�`���p�s�N�Z���V�F�[�_
/// </summary>
float4 PS_Depth(VsOutput input) :SV_Target
{
	float a = input.depth.z / input.depth.w;
	//a = (a - 0.9) * 10;
	return float4(a,0,0,1);
}
