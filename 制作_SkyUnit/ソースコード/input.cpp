//===============================================================================
//
//		 C++使った2D(input.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "input.h"
//マクロ定義

////グローバル変数
//XINPUT_STATE g_joyKeyState;
//XINPUT_STATE g_joyKeyStateTrigger;

//静的メンバ初期化
LPDIRECTINPUT8 CInput::m_pInput = nullptr;


//==========================================================================================
//コンストラクタ
//==========================================================================================
CInput::CInput()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CInput::~CInput()
{

}

//==========================================================================================
//入力初期化処理
//==========================================================================================
HRESULT CInput::Init(HINSTANCE hInstace, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstace, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//==========================================================================================
//入力終了処理
//==========================================================================================
void CInput::Uninit(void)
{
	//DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//
////キーボードのプレス情報を取得
//bool GetKeyboardPress(int nKey)
//{
//	return (g_aKeyState[nKey] & 0x80) != 0;
//}
////キーボードのプレス情報を取得
//bool GetKeyboardTrigger(int nKey)
//{
//	return (g_aKeyStateTrigger[nKey] & 0x80) != 0;
//}
//
//HRESULT InitJoypad(void)
//{
//	//メモリのクリア
//	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
//
//	//XInputEnableのステートを設定(有効にする)
//	XInputEnable(true);
//
//	return S_OK;
//}
//
//void UninitJoypad(void)
//{
//	//XInputEnableのステートを設定(無効にする)
//	XInputEnable(false);
//}
//
//void UpdateJoypad(void)
//{
//	XINPUT_STATE JoykeyState;
//	//ジョイパッドの状態を取得
//	if (XInputGetState(0, &JoykeyState) == ERROR_SUCCESS)
//	{
//		WORD Button = JoykeyState.Gamepad.wButtons;
//		WORD OldButton = g_joyKeyState.Gamepad.wButtons;
//		g_joyKeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;
//
//		g_joyKeyState = JoykeyState;
//	}
//}
//
//bool GetJoypadPress(JOYKEY key)
//{
//	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
//}
//
//bool GetJoypadTrigger(JOYKEY key)
//{
//	return (g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
//}