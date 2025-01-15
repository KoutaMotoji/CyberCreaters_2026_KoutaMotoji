//===============================================================================
//
//		C++使った2D(input.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "main.h"
#include "keyboard.h"

//静的メンバ初期化
LPDIRECTINPUTDEVICE8 CKeyboard::m_pDevKeyboard = nullptr;

//==========================================================================================
//コンストラクタ
//==========================================================================================
CKeyboard::CKeyboard() :m_aKeyState(), m_aKeyStateTrigger()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CKeyboard::~CKeyboard()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(CInput::Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	//入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//強調モードを設定
	if (FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードのアクセス権を獲得
	m_pDevKeyboard->Acquire();
	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CKeyboard::Uninit()
{
	//入力デバイスの破棄
	if (m_pDevKeyboard != NULL)
	{
		m_pDevKeyboard->Unacquire();
		m_pDevKeyboard->Release();
		m_pDevKeyboard = nullptr;
	}
	CInput::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CKeyboard::Update()
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;
	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyStateRepeat[nCntKey]  = (m_aKeyState[nCntKey] & aKeyState[nCntKey]);

			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevKeyboard->Acquire();
	}
}

//==========================================================================================
//キーボードのプレス情報を取得
//==========================================================================================
bool CKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) != 0;
}

//==========================================================================================
//キーボードのトリガー情報を取得
//==========================================================================================
bool CKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) != 0;
}

//==========================================================================================
//キーボードのリリース情報を取得
//==========================================================================================
bool CKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) != 0;
}

//==========================================================================================
//キーボードのリピート情報を取得
//==========================================================================================
bool CKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) != 0;
}