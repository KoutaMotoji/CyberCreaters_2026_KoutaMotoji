//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Akira Tanaka
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"


class CSound
{
public:
	//サウンド一覧
	typedef enum
	{
		SOUND_LABEL_TITLESE_OPEN = 0,
		SOUND_LABEL_TITLESE_LIFT,
		SOUND_LABEL_TITLESE_RIDER,
		SOUND_LABEL_TITLESE_MOVE,
		SOUND_LABEL_TITLESE_ENTER,
		SOUND_LABEL_GAMESE_EXPLOAD,
		SOUND_LABEL_GAMESE_BOMB,
		SOUND_LABEL_GAMESE_ITEM,
		SOUND_LABEL_GAMESE_SHOT,
		SOUND_LABEL_GAMESE_BOSSSHOT,
		SOUND_LABEL_GAMESE_BOSSLOCKON,
		SOUND_LABEL_GAMESE_FALL,
		SOUND_LABEL_GAMESE_SPAWN,
		SOUND_LABEL_GAMESE_TRANSFORM1,
		SOUND_LABEL_GAMESE_TRANSFORM2,
		SOUND_LABEL_GAMESE_TRANSFORM3,
		SOUND_LABEL_GAMESE_BIGBOMB,
		SOUND_LABEL_MAX

	} SOUND_LABEL;
	CSound();								//コンストラクタ
	~CSound();								//デストラクタ
	HRESULT Init(HWND hWnd);			//初期化
	void Uninit(void);					//終了
	HRESULT PlaySound(SOUND_LABEL label);	//サウンド再生
	void StopSound(SOUND_LABEL label);		//サウンド停止(ラベル指定)
	void StopSound(void);					//サウンド停止(すべて)

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
private:
	typedef struct
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	} SOUNDINFO;
	IXAudio2* m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{"data/SOUND/opener1.wav", 0},	//開く音
		{"data/SOUND/lift2.wav", 0},	//リフトが動く音
		{"data/SOUND/rider1.wav", 0},	//ブースターが上がる音
		{"data/SOUND/move1.wav", 0},	//自機が動く音
		{"data/SOUND/Enter1.wav", 0},	//Enterキーが押された音
		{"data/SOUND/expload1.wav", 0},	//爆発音１
		{"data/SOUND/expload2.wav", 0},	//爆発音２
		{"data/SOUND/Item2.wav", 0},	//アイテム獲得音
		{"data/SOUND/Shot1.wav", 0},	//射撃音
		{"data/SOUND/Shot2.wav", 0},	//ボス射撃音
		{"data/SOUND/lockon1.wav", 0},	//ボスロックオン音
		{"data/SOUND/fall.wav", 0},		//落ちる音
		{"data/SOUND/spawn1.wav", 0},	//石柱が出現する音
		{"data/SOUND/Metal1.wav", 0},	//変形時の効果音1
		{"data/SOUND/Metal2.wav", 0},	//変形時の効果音2
		{"data/SOUND/Metal3.wav", 0},	//変形時の効果音2
		{"data/SOUND/BigBomb.wav", 0},	//変形時の効果音2


	};
};

#endif
