texture2D texCampus : register(t0);
SamplerState samLinear : register(s0);



struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


/// <summary>
/// バーテックスシェーダー
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


	float2 _FlowDirection = float2(1, 1);//垂れる方向
	float2 _MainTex_TexelSize = float2(0.01, 0.01);
	float _ScaleFactor = 1;
	float _Viscosity = 0.1f;//粘り気



	//TODO:VITIATEにちっちゃいノイズ入れたい。
	const float VITIATE_X = 0.3;//どのくらい横の液体を考慮するかの係数。
	float2 shiftZ = float2(_FlowDirection.x * _MainTex_TexelSize.x, _FlowDirection.y * _MainTex_TexelSize.y) * _ScaleFactor * _Viscosity;
	float2 shiftX = float2(_FlowDirection.y * _MainTex_TexelSize.y, _FlowDirection.x * _MainTex_TexelSize.x) * _ScaleFactor * _Viscosity *VITIATE_X;
	float2 shiftx = -shiftX;
	float shift = _FlowDirection * _MainTex_TexelSize * _ScaleFactor * _Viscosity;

	//TODO:直下の高さを取ってきて、その高さに応じてどの程度流れるかを決めたい
	float2  ShiftTex = clamp(input.Tex + float2(shift, 0), 0, 1);
	float4 texZ = texCampus.Sample(samLinear, ShiftTex);
	ShiftTex = clamp(input.Tex + float2(0, shift), 0, 1);
	float4 texx = texCampus.Sample(samLinear,ShiftTex);
	ShiftTex = clamp(input.Tex + float2(0, -shift),0,1);
	float4 texX = texCampus.Sample(samLinear,ShiftTex);
	ShiftTex = clamp(input.Tex + float2(-shift, 0), 0, 1);
	float4 texz = texCampus.Sample(samLinear, ShiftTex);

	////ピクセルの液体付着量を計算
	float amountUp = texZ.a * 0.002 + texx.a * 0.002 + texX.a * 0.002+ texz.a * 0.002;//上にある液体の付着量(重みは直上優先)
																				   //上のピクセルが塗られていた場合、垂れてくると仮定して加算


	if (amountUp <= 0) {
		discard;
	}


	//垂れてきた液体を加算した合計の液体付着量
	float resultAmount = (amountUp) * 0.5;
	////垂れた液体の色を計算
	float3 maxRGB = max(texz.rgb, max(campusColor.rgb, max(texZ.rgb, max(texx.rgb, texX.rgb))));//全ての範囲の色の最大値(これで計算すると全体的に明るくなるのでまずい
	float time = clamp(amountUp - _Viscosity, 0, 1);
	float3 resultRGB = lerp(maxRGB, texZ.rgb, time);//垂れてくる液体との線形補間

	campusColor = float4(maxRGB, resultAmount);

	return campusColor;
}