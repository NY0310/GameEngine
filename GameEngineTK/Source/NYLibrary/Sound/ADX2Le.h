/// <summary>
/// ADX2を扱うライブラリ
/// </summary>
#pragma once

#include <cri_adx2le.h>
#include <memory>
#include <string>

namespace MyLibrary
{
	// ADX2Leの再生プレイヤークラス
	class ADX2Le_Player
	{
		// ACBハンドル
		CriAtomExAcbHn m_acb_hn;

		// プレイヤーハンドル
		CriAtomExPlayerHn m_player;

	public:

		// コンストラクタ
		ADX2Le_Player();

		// プレイヤーの作成
		void Create();

		// Acbファイルのロード
		void LoadAcb(const char *acb, const char *awb = NULL);

		// プレイヤーの解放関数
		void Release();

		// プレイヤーハンドルの取得
		CriAtomExPlayerHn GetPlayerHandle();

		//----- 再生 -----//

		// 指定キューの再生 
		CriAtomExPlaybackId Play(CriAtomExCueId cue_id, float volume = 1.0f);

		//----- 音量 -----//

		// プレイヤーの音量の設定
		void SetVolume(float volume);

		// 再生ID指定の音量の設定
		void SetVolumeByID(CriAtomExPlaybackId playback_id, float volume);

		//----- ポーズ -----//

		// プレイヤーのポーズ
		void Pause();

		// 再生ID指定のポーズ
		void PauseByID(CriAtomExPlaybackId playback_id);

		// プレイヤーのポーズ状態の取得
		bool IsPause();

		// 再生ID指定のポーズ状態の取得
		bool IsPauseByID(CriAtomExPlaybackId playback_id);

		//----- 停止 -----//

		// プレイヤーの停止
		void Stop();

		// 特定の再生音の停止
		void StopByID(CriAtomExPlaybackId playback_id);

	};

	// ADX2Leクラス
	class ADX2Le
	{
		// 最大ボイス数を増やすための関連パラメータ
		static const int MAX_VOICE = 24;
		static const int MAX_VIRTUAL_VOICE = 64;	// ざっくり多め(通常ボイス＋HCA-MXボイス＋α)
		static const int MAX_CRIFS_LOADER = 64;		// ざっくり多め(通常ボイス＋HCA-MXボイス＋α)

		// 最大サンプリングレート（ピッチ変更含む）
		static const int MAX_SAMPLING_RATE = (48000 * 2);

		// HCA-MXコーデックのサンプリングレート
		static const int SAMPLINGRATE_HCAMX = 32000;

		static const std::wstring RESOURCE_DIRECTORY;

		CriAtomExVoicePoolHn	m_standard_voice_pool;	// ボイスプール(ADX/HCAコーデック用)
		CriAtomExVoicePoolHn	m_hcamx_voice_pool;		// ボイスプール(HCA-MX用)
		CriAtomDbasId		m_dbas_id;				// D-BASハンドル

		ADX2Le_Player m_player; // プレイヤー

	public:
		static ADX2Le* GetInstance();
	private:
		static std::unique_ptr<ADX2Le> m_Instance;

	private:
		static void user_error_callback_func(const CriChar8 *errid, CriUint32 p1, CriUint32 p2, CriUint32 *parray);
		static void *user_alloc_func(void *obj, CriUint32 size);
		static void user_free_func(void *obj, void *ptr);

	public:

		// 初期化処理
		void Initialize(const wchar_t *acf);

		// 更新処理
		void Update();

		// 終了処理
		void Finalize();

		// プレイヤーの取得関数
		ADX2Le_Player* GetPlayer();

		// Acbファイルのロード
		void LoadAcb(const wchar_t *acb, const wchar_t *awb = NULL);

		//----- 再生 -----//

		// 指定キューの再生 
		CriAtomExPlaybackId Play(CriAtomExCueId cue_id, float volume = 1.0f);

		//----- 音量 -----//

		// プレイヤーの音量の設定
		void SetVolume(float volume);

		//----- ポーズ -----//

		// プレイヤーのポーズ
		void Pause();

		// プレイヤーのポーズ状態の取得
		bool IsPause();

		//----- 停止 -----//

		// プレイヤーの停止
		void Stop();

	};
}
