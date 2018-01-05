#pragma once

class Math
{
public:
	//2点間の距離
	static int Distance(int x1, int y1, int x2, int y2);
	//ピタゴラスの定理
	static int PythagoreanTheorem(int a, int b);
private:
	//2つの値とも0
	static bool Both0(int num1, int num2);
};