#include "Math.h"

#include<math.h>

/// <summary>
/// 2�_�Ԃ̋���
/// </summary>
/// <param name="x1">�_1��x���W</param>
/// <param name="y1">�_1��y���W</param>
/// <param name="x2">�_2��x���W</param>
/// <param name="y2">�_2��y���W</param>
int Math::Distance(int x1, int y1, int x2, int y2)
{
	//X�̍��W�̍�
	int differenceX = x1 - x2;
	//Y�̍��W�̍�
	int differenceY = y1 - y2;
	//2�̒l�Ƃ�0
	if (Both0(differenceX, differenceY))
		return 0;
	//�Εӂ�Ԃ�
	return PythagoreanTheorem(differenceX, differenceY);
}

/// <summary>
/// �s�^�S���X�̒藝
/// </summary>
/// <param name="a">����</param>
/// <param name="b">���</param>
/// <returns></returns>
int Math::PythagoreanTheorem(int a, int b)
{
	return static_cast<int>(sqrt((a * a) + (b * b)));
}

/// <summary>
/// 2�̒l�Ƃ�0
/// </summary>
/// <param name="num1">1�߂̒l</param>
/// <param name="num2">2�߂̒l</param>
/// <returns></returns>
bool Math::Both0(int num1, int num2)
{
	if(num1 == 0 && num2 == 0)
		return true;
	return false;
}
