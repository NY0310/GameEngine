#pragma once

class Math
{
public:
	//2�_�Ԃ̋���
	static int Distance(int x1, int y1, int x2, int y2);
	//�s�^�S���X�̒藝
	static int PythagoreanTheorem(int a, int b);
private:
	//2�̒l�Ƃ�0
	static bool Both0(int num1, int num2);
};