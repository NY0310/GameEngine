#include "ADX2Le.h"
#include <codecvt> 

using namespace NYLibrary;

//////////////////////////////////
// リンクするライブラリ指定		//
//////////////////////////////////
#pragma comment(lib, "cri_ware_pcx86_le_import.lib")

// 定数
// リソースディレクトリパス
const std::wstring ADX2Le::RESOURCE_DIRECTORY = L"Resources/Sounds/";

std::unique_ptr<ADX2Le> ADX2Le::m_Instance;

//--------------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------------
void ADX2Le::Initialize(const wchar_t *acf)
{
	// エラーコールバック関数の登録
	criErr_SetCallback(user_error_callback_func);

	// メモリアロケータの登録
	criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);

	// ライブラリ初期化
	CriAtomExConfig_WASAPI lib_config;
	CriFsConfig fs_config;
	criAtomEx_SetDefaultConfig_WASAPI(&lib_config);
	criFs_SetDefaultConfig(&fs_config);
	lib_config.atom_ex.max_virtual_voices = MAX_VIRTUAL_VOICE;
	lib_config.hca_mx.output_sampling_rate = SAMPLINGRATE_HCAMX;
	fs_config.num_loaders = MAX_CRIFS_LOADER;
	lib_config.atom_ex.fs_config = &fs_config;
	criAtomEx_Initialize_WASAPI(&lib_config, NULL, 0);

	// ストリーミング用バッファの作成
	m_dbas_id = criAtomDbas_Create(NULL, NULL, 0);

	// フルパスに補完
	std::wstring fullpath_wstr = RESOURCE_DIRECTORY + acf;
	// stringに変換
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::string fullpath_str = cv.to_bytes(fullpath_wstr);
	// ACFファイルの読み込みと登録
	CriBool result = criAtomEx_RegisterAcfFile(NULL, fullpath_str.c_str(), NULL, 0);

	// DSPバス設定の登録
	criAtomEx_AttachDspBusSetting("DspBusSetting_0", NULL, 0);

	// ボイスプールの作成（最大ボイス数変更／最大ピッチ変更／ストリーム再生対応）
	CriAtomExStandardVoicePoolConfig standard_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standard_vpool_config);
	standard_vpool_config.num_voices = MAX_VOICE;
	standard_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	standard_vpool_config.player_config.streaming_flag = CRI_TRUE;
	m_standard_voice_pool = criAtomExVoicePool_AllocateStandardVoicePool(&standard_vpool_config, NULL, 0);

	// HCA-MX再生用：ボイスプールの作成
	CriAtomExHcaMxVoicePoolConfig hcamx_vpool_config;
	criAtomExVoicePool_SetDefaultConfigForHcaMxVoicePool(&hcamx_vpool_config);
	hcamx_vpool_config.num_voices = MAX_VOICE;
	hcamx_vpool_config.player_config.max_sampling_rate = MAX_SAMPLING_RATE;
	hcamx_vpool_config.player_config.streaming_flag = CRI_TRUE;
	m_hcamx_voice_pool = criAtomExVoicePool_AllocateHcaMxVoicePool(&hcamx_vpool_config, NULL, 0);

	// プレイヤーの作成
	m_player.Create();
}

//--------------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------------
void ADX2Le::Finalize()
{
	// プレイヤーの破棄
	m_player.Release();

	// DSPのデタッチ
	criAtomEx_DetachDspBusSetting();

	// ボイスプールの破棄
	criAtomExVoicePool_Free(m_hcamx_voice_pool);
	criAtomExVoicePool_Free(m_standard_voice_pool);

	// ACFの登録解除
	criAtomEx_UnregisterAcf();

	// D-BASの破棄
	criAtomDbas_Destroy(m_dbas_id);

	// ライブラリの終了
	criAtomEx_Finalize_WASAPI();
}

//--------------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------------
void ADX2Le::Update()
{
	criAtomEx_ExecuteMain();
}

ADX2Le * ADX2Le::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance.reset(new ADX2Le);
	}

	return m_Instance.get();
}

//--------------------------------------------------------------------------------------
// エラーコールバック関数のユーザ実装
//--------------------------------------------------------------------------------------
void ADX2Le::user_error_callback_func(const CriChar8 *errid, CriUint32 p1, CriUint32 p2, CriUint32 *parray)
{
	const CriChar8 *errmsg;

	// エラーコードからエラー文字列に変換してコンソール出力する
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);

	const unsigned int dataSize = 256;
	wchar_t wlocal[dataSize + 1] = { 0x00 };

	// マルチバイト文字をunicodeへ変換する
	MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		errmsg,
		dataSize,
		wlocal,
		dataSize + 1);

	// コンソール出力する
	OutputDebugString(wlocal);

	return;
}

//--------------------------------------------------------------------------------------
//	メモリ確保関数のユーザ実装
//--------------------------------------------------------------------------------------
void *ADX2Le::user_alloc_func(void *obj, CriUint32 size)
{
	void *ptr;
	ptr = malloc(size);
	return ptr;
}

//--------------------------------------------------------------------------------------
// メモリ解放関数のユーザ実装
//--------------------------------------------------------------------------------------
void ADX2Le::user_free_func(void *obj, void *ptr)
{
	free(ptr);
}

//--------------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------------
ADX2Le_Player::ADX2Le_Player()
{
	// Acbハンドルの初期化
	m_acb_hn = nullptr;

	// プレイヤーの初期化
	m_player = nullptr;
}

//--------------------------------------------------------------------------------------
// プレイヤーの作成
//--------------------------------------------------------------------------------------
void ADX2Le_Player::Create()
{
	// プレイヤーの作成
	if (m_player == nullptr) m_player = criAtomExPlayer_Create(NULL, NULL, 0);
}

//--------------------------------------------------------------------------------------
// Acbファイルのロード
//--------------------------------------------------------------------------------------
void ADX2Le_Player::LoadAcb(const char *acb, const char *awb)
{
		// ACBファイル読み込み
	if (m_acb_hn != nullptr)
	{
		// ACBハンドルの破棄
		criAtomExAcb_Release(m_acb_hn);
	}
	m_acb_hn = criAtomExAcb_LoadAcbFile(NULL, acb, NULL, awb, NULL, 0);
}

//--------------------------------------------------------------------------------------
// プレイヤーの解放関数
//--------------------------------------------------------------------------------------
void ADX2Le_Player::Release()
{
	// プレーヤハンドルの破棄
	if (m_player != nullptr) criAtomExPlayer_Destroy(m_player);
	// ACBハンドルの破棄
	if (m_acb_hn != nullptr) criAtomExAcb_Release(m_acb_hn);
}

//--------------------------------------------------------------------------------------
// プレイヤーハンドルの取得
//--------------------------------------------------------------------------------------
CriAtomExPlayerHn ADX2Le_Player::GetPlayerHandle()
{
	return m_player;
}

//--------------------------------------------------------------------------------------
// 指定キューの再生 
//--------------------------------------------------------------------------------------
CriAtomExPlaybackId ADX2Le_Player::Play(CriAtomExCueId cue_id, float volume)
{
	// 音量の設定
	criAtomExPlayer_SetVolume(m_player, volume);
	// キューIDの指定
	criAtomExPlayer_SetCueId(m_player, m_acb_hn, cue_id);

	// 再生の開始
	return criAtomExPlayer_Start(m_player);
}

//--------------------------------------------------------------------------------------
// プレイヤーの音量の設定
//--------------------------------------------------------------------------------------
void ADX2Le_Player::SetVolume(float volume)
{
	criAtomExPlayer_SetVolume(m_player, volume);
	criAtomExPlayer_UpdateAll(m_player);
}

//--------------------------------------------------------------------------------------
// 再生ID指定の音量の設定
//--------------------------------------------------------------------------------------
void ADX2Le_Player::SetVolumeByID(CriAtomExPlaybackId playback_id, float volume)
{
	criAtomExPlayer_SetVolume(m_player, volume);
	criAtomExPlayer_Update(m_player, playback_id);
}

//--------------------------------------------------------------------------------------
// プレイヤーのポーズ
//--------------------------------------------------------------------------------------
void ADX2Le_Player::Pause()
{
	// ポーズ中？
	if (criAtomExPlayer_IsPaused(m_player) == CRI_TRUE)
	{
		// ポーズ解除
		criAtomExPlayer_Pause(m_player, CRI_FALSE);
	}
	else
	{
		// ポーズ
		criAtomExPlayer_Pause(m_player, CRI_TRUE);
	}
}

//--------------------------------------------------------------------------------------
// 再生ID指定のポーズ
//--------------------------------------------------------------------------------------
void ADX2Le_Player::PauseByID(CriAtomExPlaybackId playback_id)
{
	// ポーズ中？
	if (criAtomExPlayback_IsPaused(playback_id) == CRI_TRUE)
	{
		// ポーズ解除
		criAtomExPlayback_Pause(playback_id, CRI_FALSE);
	}
	else
	{
		// ポーズ
		criAtomExPlayback_Pause(playback_id, CRI_TRUE);
	}
}

//--------------------------------------------------------------------------------------
// プレイヤーのポーズ状態の取得
//--------------------------------------------------------------------------------------
bool ADX2Le_Player::IsPause()
{
	if (criAtomExPlayer_IsPaused(m_player) == CRI_TRUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------
// 再生ID指定のポーズ状態の取得
//--------------------------------------------------------------------------------------
bool ADX2Le_Player::IsPauseByID(CriAtomExPlaybackId playback_id)
{
	if (criAtomExPlayback_IsPaused(playback_id) == CRI_TRUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------
// プレイヤーの停止
//--------------------------------------------------------------------------------------
void ADX2Le_Player::Stop()
{
	criAtomExPlayer_Stop(m_player);
}

//--------------------------------------------------------------------------------------
// 特定の再生音の停止
//--------------------------------------------------------------------------------------
void ADX2Le_Player::StopByID(CriAtomExPlaybackId playback_id)
{
	criAtomExPlayback_Stop(playback_id);
}

//--------------------------------------------------------------------------------------
// プレイヤーの取得関数
//--------------------------------------------------------------------------------------
ADX2Le_Player* ADX2Le::GetPlayer()
{
	return &m_player;
}

//--------------------------------------------------------------------------------------
// Acbファイルのロード
//--------------------------------------------------------------------------------------
void ADX2Le::LoadAcb(const wchar_t *acb, const wchar_t *awb)
{
	// フルパスに補完
	std::wstring fullpath_acb_wstr = RESOURCE_DIRECTORY + acb;
	std::wstring fullpath_awb_wstr = RESOURCE_DIRECTORY + awb;
	// stringに変換
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;
	std::string fullpath_acb_str = cv.to_bytes(fullpath_acb_wstr);
	std::string fullpath_awb_str = cv.to_bytes(fullpath_awb_wstr);

	m_player.LoadAcb(fullpath_acb_str.c_str(), fullpath_awb_str.c_str());
}

//--------------------------------------------------------------------------------------
// 指定キューの再生 
//--------------------------------------------------------------------------------------
CriAtomExPlaybackId ADX2Le::Play(CriAtomExCueId cue_id, float volume)
{
	return m_player.Play(cue_id, volume);
}

//--------------------------------------------------------------------------------------
// プレイヤーの音量の設定
//--------------------------------------------------------------------------------------
void ADX2Le::SetVolume(float volume)
{
	m_player.SetVolume(volume);
}

//--------------------------------------------------------------------------------------
// プレイヤーのポーズ
//--------------------------------------------------------------------------------------
void ADX2Le::Pause()
{
	m_player.Pause();
}

//--------------------------------------------------------------------------------------
// プレイヤーのポーズ状態の取得
//--------------------------------------------------------------------------------------
bool ADX2Le::IsPause()
{
	return m_player.IsPause();
}

//--------------------------------------------------------------------------------------
// プレイヤーの停止
//--------------------------------------------------------------------------------------
void ADX2Le::Stop()
{
	m_player.Stop();
}

//--------------------------------------------------------------------------------------
// 指定キューの再生 
//--------------------------------------------------------------------------------------
void ADX2Le::Stop(CriAtomExCueId cue_id)
{
	m_player.StopByID(cue_id);
}

//--------------------------------------------------------------------------------------
// 指定キューの状態を取得
//--------------------------------------------------------------------------------------
bool ADX2Le::IsPauseByID(CriAtomExCueId cue_id)
{
	return m_player.IsPauseByID(cue_id);
}

