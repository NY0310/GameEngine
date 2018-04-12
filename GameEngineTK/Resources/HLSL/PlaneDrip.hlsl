texture2D texCampus : register(t0);
SamplerState samLinear : register(s0);



struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


/// <summary>
/// �o�[�e�b�N�X�V�F�[�_�[
/// </summary>
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Tex = Tex;
	output.Pos = Pos;
	return output;
}



float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 campusColor = texCampus.Sample(samLinear, input.Tex);


	float2 _FlowDirection = float2(1, 1);//��������
	float2 _MainTex_TexelSize = float2(0.01, 0.01);
	float _ScaleFactor = 1;
	float _Viscosity = 0.1f;//�S��C



	//TODO:VITIATE�ɂ������Ⴂ�m�C�Y���ꂽ���B
	const float VITIATE_X = 0.3;//�ǂ̂��炢���̉t�̂��l�����邩�̌W���B
	float2 shiftZ = float2(_FlowDirection.x * _MainTex_TexelSize.x, _FlowDirection.y * _MainTex_TexelSize.y) * _ScaleFactor * _Viscosity;
	float2 shiftX = float2(_FlowDirection.y * _MainTex_TexelSize.y, _FlowDirection.x * _MainTex_TexelSize.x) * _ScaleFactor * _Viscosity *VITIATE_X;
	float2 shiftx = -shiftX;
	float shift = _FlowDirection * _MainTex_TexelSize * _ScaleFactor * _Viscosity;

	//TODO:�����̍���������Ă��āA���̍����ɉ����Ăǂ̒��x����邩�����߂���
	float2  ShiftTex = clamp(input.Tex + float2(shift, 0), 0, 1);
	float4 texZ = texCampus.Sample(samLinear, ShiftTex);
	ShiftTex = clamp(input.Tex + float2(0, shift), 0, 1);
	float4 texx = texCampus.Sample(samLinear,ShiftTex);
	ShiftTex = clamp(input.Tex + float2(0, -shift),0,1);
	float4 texX = texCampus.Sample(samLinear,ShiftTex);
	ShiftTex = clamp(input.Tex + float2(-shift, 0), 0, 1);
	float4 texz = texCampus.Sample(samLinear, ShiftTex);

	////�s�N�Z���̉t�̕t���ʂ��v�Z
	float amountUp = texZ.a * 0.002 + texx.a * 0.002 + texX.a * 0.002+ texz.a * 0.002;//��ɂ���t�̂̕t����(�d�݂͒���D��)
																				   //��̃s�N�Z�����h���Ă����ꍇ�A����Ă���Ɖ��肵�ĉ��Z


	if (amountUp <= 0) {
		discard;
	}


	//����Ă����t�̂����Z�������v�̉t�̕t����
	float resultAmount = (amountUp) * 0.5;
	////���ꂽ�t�̂̐F���v�Z
	float3 maxRGB = max(texz.rgb, max(campusColor.rgb, max(texZ.rgb, max(texx.rgb, texX.rgb))));//�S�Ă͈̔͂̐F�̍ő�l(����Ōv�Z����ƑS�̓I�ɖ��邭�Ȃ�̂ł܂���
	float time = clamp(amountUp - _Viscosity, 0, 1);
	float3 resultRGB = lerp(maxRGB, texZ.rgb, time);//����Ă���t�̂Ƃ̐��`���

	campusColor = float4(maxRGB, resultAmount);

	return campusColor;
}