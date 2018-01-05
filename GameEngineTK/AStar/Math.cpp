#include "Math.h"

#include<math.h>

/// <summary>
/// 2点間の距離
/// </summary>
/// <param name="x1">点1のx座標</param>
/// <param name="y1">点1のy座標</param>
/// <param name="x2">点2のx座標</param>
/// <param name="y2">点2のy座標</param>
int Math::Distance(int x1, int y1, int x2, int y2)
{
	//Xの座標の差
	int differenceX = x1 - x2;
	//Yの座標の差
	int differenceY = y1 - y2;
	//2つの値とも0
	if (Both0(differenceX, differenceY))
		return 0;
	//斜辺を返す
	return PythagoreanTheorem(differenceX, differenceY);
}

/// <summary>
/// ピタゴラスの定理
/// </summary>
/// <param name="a">高さ</param>
/// <param name="b">底辺</param>
/// <returns></returns>
int Math::PythagoreanTheorem(int a, int b)
{
	return static_cast<int>(sqrt((a * a) + (b * b)));
}

/// <summary>
/// 2つの値とも0
/// </summary>
/// <param name="num1">1つめの値</param>
/// <param name="num2">2つめの値</param>
/// <returns></returns>
bool Math::Both0(int num1, int num2)
{
	if(num1 == 0 && num2 == 0)
		return true;
	return false;
}
