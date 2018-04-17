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
	float4 speed = float4(0,0,0,0);//���x
	float frame;//�t���[����
};

cbuffer global_1:register(b1)
{
	float4 ambient = float4(0, 0, 0, 1.0f);//�A���r�G���g��
	float4 diffuse = float4(0.78f, 0.78f, 0.78f, 1); //�g�U����(�F�j
	float4 specular = float4(0.4f, 0.4f, 0.4f, 1.0f);//���ʔ���

};


#define		_PI				3.1415926535f
#define		GRAVITY			0.00f
#define		LIMITPLANE		40.0f
#define		MODELRADIUS		20.0f
#define		FACE_VELOCITY	0.5f
#define		MAT_DIFFUSECOLOR	float3( 1.0f, 1.0f, 0.0f )
#define		MAT_SPECULARCOLOR	float3( 1.0f, 1.0f, 1.0f )


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
	output.Pos = Pos;
	//�@�������f���̉�]�ɍ��킹��
	output.Normal = Norm;
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


//	��]���v�Z����֐�
void SetRotate(float3 rotang, out float3x3 mat)
{
	float2	roty;
	float2	rotz;
	sincos(rotang.y, roty.x, roty.y);
	sincos(rotang.z, rotz.x, rotz.y);
	mat._11 = rotz.y * roty.y;
	mat._12 = rotz.x * roty.y;
	mat._13 = -roty.x;
	mat._21 = -rotz.x;
	mat._22 = rotz.y;
	mat._23 = 0.0f;
	mat._31 = roty.x;
	mat._32 = 0.0f;
	mat._33 = roty.y;
}

bool IsSpeedZero(float4 speed)
{
	if (speed.x <= 0)
		return true;
	if (speed.y <= 0)
		return true;
	if (speed.z <= 0)
		return true;
	return false;
}


[maxvertexcount(3)]
void GS(triangle VS_OUTPUT In[3],
	inout TriangleStream<VS_OUTPUT> gsstream)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	static const float3	cV0 = {
		0.0f,	0.0f,	0.0f,
	};
	static const float3	cAG = {
		0.0f,	GRAVITY,	0.0f,
	};

	//�|���S���̒������W���擾
	float3	center = (In[0].Pos.xyz + In[1].Pos.xyz + In[2].Pos.xyz) * (1.0f / 3.0f);


	//���K���@�������߂�
	float3	v0 = In[1].Pos.xyz - In[0].Pos.xyz;
	float3	v1 = In[2].Pos.xyz - In[0].Pos.xyz;
	float3	fnormal = normalize(cross(v0, v1));

	//�|���S���̉�]��
	float3	ang = float3(0, 0, 0);
	//�d��
	float3	fallSpeed = float3(0, 0, 0);
	
	float3	faceSpeed = float3(0, 0, 0);

	//	���Ԃ����炷
	//float	t = frame - (MODELRADIUS - center.y) * LIMITPLANE;
	//	�S�������ɔj��
	float t = frame;

	//0�ȉ��͖���
	if (frame > 0) {
		//��]�ʂ��v�Z
		float t2 = t * t;
		fallSpeed = cV0 * t + cAG * t2;
		ang.z = frame * 0.13f;
		ang.y = frame * 0.013f * In[1].Pos.x;
		ang.x = 0.0f;
		if (IsSpeedZero(speed))
		{
			faceSpeed = FACE_VELOCITY * fnormal * t;
		}
		else
		{
			faceSpeed = FACE_VELOCITY * speed * t;
		}
	}

	float3x3 rotmat;
	//��]�ʂ����]�s���
	SetRotate(ang, rotmat);

	VS_OUTPUT CIn[3];

	for (int j = 0; j < 3; j++) {
		CIn[j] = In[j];
	}

	for (int i = 0; i < 3; i++) {
		//���S����̋���
		CIn[i].Pos.xyz -= center;
		//�����ɉ�]�s���K��������
		float3	newpos = mul(CIn[i].Pos.xyz, rotmat);
		//�ړ���̍��W�����߂�
		CIn[i].Pos.xyz = center + newpos.xyz + faceSpeed;
		//�@������]������
		CIn[i].Normal = mul(CIn[i].Normal, rotmat);
		//���[���h�s������߂�
		float4	wpos = mul(CIn[i].Pos, mW);
		//�ړ��ʂ���������
		wpos.xyz += fallSpeed;
		Out.PosWorld = wpos;
		//���[���h�E�r���[�E�v���W�F�N�V�����s��
		Out.Pos = mul(CIn[i].Pos, mWVP);
		Out.Normal = mul(CIn[i].Normal, (float3x3)mW);
		//UV
		Out.Tex = CIn[i].Tex;

		gsstream.Append(Out);
	}

	gsstream.RestartStrip();
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