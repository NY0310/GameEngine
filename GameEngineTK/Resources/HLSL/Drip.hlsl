texture2D texCampus : register(t0);
SamplerState samLinear : register(s0);



struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


/// <summary>
/// �������ǂ���
/// </summary>
bool IsOpacity(float color) {
	return (!color <= 0);
}


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


	float2 _FlowDirection =float2(0, -1);//��������
	float2 _MainTex_TexelSize = float2(0.05, 0.05);
	float _ScaleFactor =1;
	float _Viscosity = 0.08f;//�S��C



	const float VITIATE_X = 0.3;//�ǂ̂��炢���̉t�̂��l�����邩�̌W���B
	float2 shiftZ = float2(_FlowDirection.x * _MainTex_TexelSize.x, _FlowDirection.y * _MainTex_TexelSize.y) * _ScaleFactor * _Viscosity;
	float2 shiftX = float2(_FlowDirection.y * _MainTex_TexelSize.x, _FlowDirection.x * _MainTex_TexelSize.y) * _ScaleFactor * _Viscosity *VITIATE_X;
	float2 shiftx = -shiftX;

	//��A����A�E��̃C���N�̍���������
	float2  ShiftTex = clamp(input.Tex + shiftZ, 0, 1);
	float4 texZ = texCampus.Sample(samLinear, ShiftTex);
	ShiftTex = clamp(input.Tex + shiftx + shiftZ, 0, 1);
	float4 texx = texCampus.Sample(samLinear,ShiftTex);
	ShiftTex = clamp(input.Tex + shiftX + shiftZ,0,1) ;
	float4 texX = texCampus.Sample(samLinear,ShiftTex);

	//�s�N�Z���̉t�̕t����
	float amountUp = texZ.a * 0.5 + texx.a * 0.25 + texX.a * 0.25;

	//�s�N�Z���ɃC���N���h���Ȃ�����
	if (amountUp <= 0) {
		discard;
	}


	//����Ă����t�̂����Z�������v�̉t�̕t����
	float resultAmount = (campusColor.a  + amountUp) * 0.5;
	//���ꂽ�t�̂̐F���v�Z
	//��A����A�E��̐F�ň�Ԗ��邢�F
	float3 maxRGB = max(campusColor.rgb, max(texZ.rgb, max(texx.rgb, texX.rgb)));
	//float3 maxRGB = max(texZ.rgb, b, texX.rgb));

	float time = clamp(amountUp - _Viscosity, 0, 1);
	//����Ă���t�̂Ƃ̕��
	float3 resultRGB = lerp(maxRGB, texZ.rgb, time);

	campusColor = float4(resultRGB, resultAmount);

	return campusColor;
}