/// <summary>
/// 子供として登録することで自動でLerp処理を行う
/// 中田　湧介
/// 2018/04/19
/// </summary>
#pragma once
#include "../SceneGraph/Node/GameObjectNode.h"
#include "../Math/Math.h"

namespace NYLibrary
{
	class Lerp : public GameObjectNode
	{
	public:
		//始点、終点、プレイ時間
		Lerp(float start, float end, float playTime) :
			start(start), end(end), frameCnt(0.0f), playTime(playTime) {}
		
		//更新
		void Update()
		{
			if (useUpdateTime)
			{
				frameCnt++;


				time = frameCnt / playTime;
			}
			
			now = Math::Lerp(start, end, time);

			if (time >= 1.0f)
			{
				DisableUpdate();
				listener();
			}
		//	useUpdateTime = true;
		}
		//時間をリセット
		void TimeReset()
		{
			frameCnt = 0;
		}

		//現在の値を取得
		float GetNow() { return now; }
		//進行度を取得
		float GetTime() { return time; }
		//進行度を設定
		void SetTime(float time) { this->time = time; useUpdateTime = false; }
		//最初を設定
		void SetStart(float start) { this->start = start; useUpdateTime = true; }
		//最終を設定
		void SetEnd(float end) { this->end = end; }
		//ラープが終了したか
		bool IsLerpEnd() { return time >= 1; DisableUpdate(); }
		//処理を受け取る
		void addListener(std::function<void()> listener) {
			this->listener = listener;
		}
		//クリア
		void Clear() { frameCnt = 0; now = start; Update(); }
	private:
		float start;//始点
		float end;//終点
		float frameCnt;//フレームカウント
		const float playTime;//実行時間
		float now;//現在
		float time;//進行度
		std::function<void()> listener = [&]() {};				   //ラムダ式を受け取る
		bool useUpdateTime;
	};
}