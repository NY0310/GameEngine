#include "Charactor.h"

using namespace std;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="maxHp">体力上限</param>
/// <param name="matrixObject">オブジェクト行列</param>
Charactor::Charactor(int maxHp)
	:MAX_HP(maxHp)
{
}



/// <summary>
/// デストラクタ
/// </summary>
Charactor::~Charactor()
{
	Finalize();
}

/// <summary>
/// 初期化関数
/// </summary>
void Charactor::Initialize()
{
	matrixObject = make_unique<MatrixObject>();
}

/// <summary>
/// 終了関数
/// </summary>
void Charactor::Finalize()
{
	matrixObject.release();
}
