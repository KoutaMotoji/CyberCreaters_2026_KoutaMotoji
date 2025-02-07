//===============================================================================
//
//		C++使った2D(input.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "main.h"
#include "joypad.h"

namespace
{
	int LEFT_JOYSTICK_DEADZONE = 3500;
	int RIGHT_JOYSTICK_DEADZONE = 4200;
}

//==========================================================================================
//コンストラクタ
//==========================================================================================
CJoypad::CJoypad()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CJoypad::~CJoypad()
{

}

//==========================================================================================
//コントローラーの初期化
//==========================================================================================
HRESULT CJoypad::Init(HINSTANCE hInstace, HWND hWnd)
{
	//メモリのクリア
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));

	//XInputEnableのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//==========================================================================================
//コントローラーの終了処理
//==========================================================================================
void CJoypad::Uninit()
{
	//XInputEnableのステートを設定(無効にする)
	XInputEnable(false);
}

//==========================================================================================
//コントローラーの更新処理
//==========================================================================================
void CJoypad::Update()
{
	XINPUT_STATE JoykeyState;
	//ジョイパッドの状態を取得
	if (XInputGetState(0, &JoykeyState) == ERROR_SUCCESS)
	{
		WORD Button = JoykeyState.Gamepad.wButtons;
		BYTE LeftTrigger = JoykeyState.Gamepad.bLeftTrigger;    // 左トリガーアナログコントロール
		BYTE RightTrigger = JoykeyState.Gamepad.bRightTrigger;   // 右トリガーアナログコントロール
		SHORT ThumbLX = JoykeyState.Gamepad.sThumbLX;       // 左スティックの X 軸
		SHORT ThumbLY = JoykeyState.Gamepad.sThumbLY;       // 左スティックの Y 軸
		SHORT ThumbRX = JoykeyState.Gamepad.sThumbRX;       // 右スティックの X 軸
		SHORT ThumbRY = JoykeyState.Gamepad.sThumbRY;       // 右スティックの Y 軸

		WORD OldButton = m_joyKeyState.Gamepad.wButtons;
		WORD OldLeftTrigger = m_joyKeyState.Gamepad.bLeftTrigger;
		WORD OldRightTrigger = m_joyKeyState.Gamepad.bRightTrigger;
		m_joyKeyOldState = m_joyKeyState;


		m_joyKeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;
		m_joyKeyStateTrigger.Gamepad.bLeftTrigger = ~OldLeftTrigger & LeftTrigger;
		m_joyKeyStateTrigger.Gamepad.bRightTrigger = ~OldRightTrigger & RightTrigger;

		m_joyKeyStateRelease.Gamepad.wButtons = OldButton & ~Button;
		m_joyKeyStateRelease.Gamepad.bLeftTrigger = OldLeftTrigger & ~LeftTrigger;
		m_joyKeyStateRelease.Gamepad.bRightTrigger = OldRightTrigger & ~RightTrigger;

		m_joyKeyStateRepeat.Gamepad.wButtons = OldButton & Button;
		m_joyKeyStateRepeat.Gamepad.bLeftTrigger = OldLeftTrigger & LeftTrigger;
		m_joyKeyStateRepeat.Gamepad.bRightTrigger = OldRightTrigger & RightTrigger;


		//XINPUT_STATE state;
		//ZeroMemory(&state, sizeof(XINPUT_STATE));
		// 各スティックの入力値がデッドゾーン以下なら入力値を0にする
		if ((JoykeyState.Gamepad.sThumbLX <  LEFT_JOYSTICK_DEADZONE &&
			JoykeyState.Gamepad.sThumbLX > -LEFT_JOYSTICK_DEADZONE) &&
			(JoykeyState.Gamepad.sThumbLY <  LEFT_JOYSTICK_DEADZONE &&
				JoykeyState.Gamepad.sThumbLY > -LEFT_JOYSTICK_DEADZONE))
		{//左スティックがデッドゾーン内なので数値を無効
			JoykeyState.Gamepad.sThumbLX = 0;
			JoykeyState.Gamepad.sThumbLY = 0;
		}

		if ((JoykeyState.Gamepad.sThumbRX <  RIGHT_JOYSTICK_DEADZONE &&
			JoykeyState.Gamepad.sThumbRX >  -RIGHT_JOYSTICK_DEADZONE) &&
			(JoykeyState.Gamepad.sThumbRY <  RIGHT_JOYSTICK_DEADZONE &&
				JoykeyState.Gamepad.sThumbRY >  -RIGHT_JOYSTICK_DEADZONE))
		{//右スティックがデッドゾーン内なので数値を無効
			JoykeyState.Gamepad.sThumbRX = 0;
			JoykeyState.Gamepad.sThumbRY = 0;
		}
		m_joyKeyState = JoykeyState;

	}
}

//==========================================================================================
//コントローラーのプレス情報
//==========================================================================================
bool CJoypad::GetPress(JOYKEY key)
{
	//もし入力されたボタンがLT・RTならボタン入力に変換する
	if (key == JOYPAD_LEFT_TRIGGER)
	{
		return (m_joyKeyState.Gamepad.bLeftTrigger & key) ? true : false;
	}
	else if (key == JOYPAD_RIGHT_TRIGGER)
	{
		return (m_joyKeyState.Gamepad.bRightTrigger & key) ? true : false;
	}
	else
	{
		return (m_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
	}
}

//==========================================================================================
//コントローラーのトリガー情報
//==========================================================================================
bool CJoypad::GetTrigger(JOYKEY key)
{
	//もし入力されたボタンがLT・RTならボタン入力に変換する
	if (key == JOYPAD_LEFT_TRIGGER)
	{
		return (m_joyKeyStateTrigger.Gamepad.bLeftTrigger & key) ? true : false;
	}
	else if (key == JOYPAD_RIGHT_TRIGGER)
	{
		return (m_joyKeyStateTrigger.Gamepad.bRightTrigger & key) ? true : false;
	}
	else
	{
		return (m_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
	}
}

//==========================================================================================
//コントローラーのリリース(離した時)情報
//==========================================================================================
bool CJoypad::GetRelease(JOYKEY key)
{
	//もし入力されたボタンがLT・RTならボタン入力に変換する
	if (key == JOYPAD_LEFT_TRIGGER)
	{
		return (m_joyKeyStateRelease.Gamepad.bLeftTrigger & key) ? true : false;
	}
	else if (key == JOYPAD_RIGHT_TRIGGER)
	{
		return (m_joyKeyStateRelease.Gamepad.bRightTrigger & key) ? true : false;
	}
	else
	{
		return (m_joyKeyStateRelease.Gamepad.wButtons & (0x01 << key)) ? true : false;
	}
}

//==========================================================================================
//コントローラーのリピート(押され続けている)情報
//==========================================================================================
bool CJoypad::GetRepeat(JOYKEY key)
{

	//もし入力されたボタンがLT・RTならボタン入力に変換する
	if (key == JOYPAD_LEFT_TRIGGER)
	{
		return (m_joyKeyStateRepeat.Gamepad.bLeftTrigger & key) ? true : false;
	}
	else if (key == JOYPAD_RIGHT_TRIGGER)
	{
		return (m_joyKeyStateRepeat.Gamepad.bRightTrigger & key) ? true : false;
	}
	else
	{
		return (m_joyKeyStateRepeat.Gamepad.wButtons & (0x01 << key)) ? true : false;
	}
}

//==========================================================================================
//左スティックの取得
//==========================================================================================
bool CJoypad::GetJoyStickL(JOYSTICK joystick)
{
	switch (joystick)
	{
	case JOYSTICK_DLEFT:
		return m_joyKeyState.Gamepad.sThumbLX <= -LEFT_JOYSTICK_DEADZONE;
		break;
	case JOYSTICK_DRIGHT:
		return m_joyKeyState.Gamepad.sThumbLX >= LEFT_JOYSTICK_DEADZONE;
		break;
	case JOYSTICK_DUP:
		return m_joyKeyState.Gamepad.sThumbLY >= LEFT_JOYSTICK_DEADZONE;
		break;
	case JOYSTICK_DDOWN:
		return m_joyKeyState.Gamepad.sThumbLY <= -LEFT_JOYSTICK_DEADZONE;
		break;
	}
}

//==========================================================================================
//右スティックの取得
//==========================================================================================
bool CJoypad::GetJoyStickR(JOYSTICK joystick)
{
	switch (joystick)
	{
	case JOYSTICK_DLEFT:
		return m_joyKeyState.Gamepad.sThumbRX <= -RIGHT_JOYSTICK_DEADZONE;
		break;
	case JOYSTICK_DRIGHT:
		return m_joyKeyState.Gamepad.sThumbRX >= RIGHT_JOYSTICK_DEADZONE;
		break;
	case JOYSTICK_DUP:
		return m_joyKeyState.Gamepad.sThumbRY >= RIGHT_JOYSTICK_DEADZONE;
		break;
	case JOYSTICK_DDOWN:
		return m_joyKeyState.Gamepad.sThumbRY <= -RIGHT_JOYSTICK_DEADZONE;
		break;
	}
}

//==========================================================================================
//左スティックの方向ベクトルを取得
//==========================================================================================
D3DXVECTOR2 CJoypad::GetJoyStickVecL()
{
	// 各スティックの入力値がデッドゾーン以下なら入力値を0にする
	if ((m_joyKeyState.Gamepad.sThumbLX >  LEFT_JOYSTICK_DEADZONE ||
		m_joyKeyState.Gamepad.sThumbLX < -LEFT_JOYSTICK_DEADZONE) ||
		(m_joyKeyState.Gamepad.sThumbLY >  LEFT_JOYSTICK_DEADZONE ||
			m_joyKeyState.Gamepad.sThumbLY < -LEFT_JOYSTICK_DEADZONE))
	{//左スティック
		float VecX = sinf(atan2f((float)m_joyKeyState.Gamepad.sThumbLX * 0.01f, (float)m_joyKeyState.Gamepad.sThumbLY * 0.01f));
		float VecY = cosf(atan2f((float)m_joyKeyState.Gamepad.sThumbLX * 0.01f, (float)m_joyKeyState.Gamepad.sThumbLY * 0.01f));
		return { VecX ,VecY };
	}
	else
	{
		return {0.0f,0.0f};
	}

}
//==========================================================================================
//右スティックの方向ベクトルを取得
//==========================================================================================
D3DXVECTOR2 CJoypad::GetJoyStickVecR()
{
	// 各スティックの入力値がデッドゾーン以下なら入力値を0にする
	if ((m_joyKeyState.Gamepad.sThumbRX > RIGHT_JOYSTICK_DEADZONE ||
		m_joyKeyState.Gamepad.sThumbRX < -RIGHT_JOYSTICK_DEADZONE) ||
		(m_joyKeyState.Gamepad.sThumbRY > RIGHT_JOYSTICK_DEADZONE ||
			m_joyKeyState.Gamepad.sThumbRY < -RIGHT_JOYSTICK_DEADZONE))
	{//左スティック
		float VecX = sinf(atan2f((float)m_joyKeyState.Gamepad.sThumbRX * 0.01f, (float)m_joyKeyState.Gamepad.sThumbRY * 0.01f));
		float VecY = cosf(atan2f((float)m_joyKeyState.Gamepad.sThumbRX * 0.01f, (float)m_joyKeyState.Gamepad.sThumbRY * 0.01f));
		return { VecX ,VecY };
	}
	else
	{
		return { 0.0f,0.0f };
	}

}
//==========================================================================================
//スティックのトリガー判定の取得
//==========================================================================================
bool CJoypad::GetJoyStickTrigger(JOYKEY joykey, JOYSTICK joystick)
{
	if (joykey == JOYPAD_LEFT_THUMB)
	{
		switch (joystick)
		{
		case JOYSTICK_DLEFT:
			return (m_joyKeyState.Gamepad.sThumbLX <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLX >= -RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DRIGHT:
			return (m_joyKeyState.Gamepad.sThumbLX >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLX <= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DUP:
			return (m_joyKeyState.Gamepad.sThumbLY >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLY <= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DDOWN:
			return (m_joyKeyState.Gamepad.sThumbLY <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLY >= -RIGHT_JOYSTICK_DEADZONE);
			break;
		}
	}
	else if (joykey == JOYPAD_RIGHT_THUMB)
	{
		switch (joystick)
		{
		case JOYSTICK_DLEFT:
			return (m_joyKeyState.Gamepad.sThumbRX <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRX >= -RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DRIGHT:
			return (m_joyKeyState.Gamepad.sThumbRX >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRX <= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DUP:
			return (m_joyKeyState.Gamepad.sThumbRY >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRY <= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DDOWN:
			return (m_joyKeyState.Gamepad.sThumbRY <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRY >= -RIGHT_JOYSTICK_DEADZONE);
			break;
		}
	}
}
//==========================================================================================
//スティックのリリース判定の取得
//==========================================================================================
bool CJoypad::GetJoyStickRelease(JOYKEY joykey, JOYSTICK joystick)
{
	if (joykey == JOYPAD_LEFT_THUMB)
	{
		switch (joystick)
		{
		case JOYSTICK_DLEFT:
			return (m_joyKeyState.Gamepad.sThumbLX >= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLX <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DRIGHT:
			return (m_joyKeyState.Gamepad.sThumbLX <= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLX >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DUP:
			return (m_joyKeyState.Gamepad.sThumbLY <= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLY >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DDOWN:
			return (m_joyKeyState.Gamepad.sThumbLY >= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLY <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		}
	}
	else if (joykey == JOYPAD_RIGHT_THUMB)
	{
		switch (joystick)
		{
		case JOYSTICK_DLEFT:
			return (m_joyKeyState.Gamepad.sThumbRX >= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRX <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DRIGHT:
			return (m_joyKeyState.Gamepad.sThumbRX <= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRX >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DUP:
			return (m_joyKeyState.Gamepad.sThumbRY <= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRY >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DDOWN:
			return (m_joyKeyState.Gamepad.sThumbRY >= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRY <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		}
	}
}
//==========================================================================================
//スティックのリピート判定の取得
//==========================================================================================
bool CJoypad::GetJoyStickRepeat(JOYKEY joykey, JOYSTICK joystick)
{
	if (joykey == JOYPAD_LEFT_THUMB)
	{
		switch (joystick)
		{
		case JOYSTICK_DLEFT:
			return (m_joyKeyState.Gamepad.sThumbLX <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLX <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DRIGHT:
			return (m_joyKeyState.Gamepad.sThumbLX >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLX >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DUP:
			return (m_joyKeyState.Gamepad.sThumbLY >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLY >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DDOWN:
			return (m_joyKeyState.Gamepad.sThumbLY <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbLY <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		}
	}
	else if (joykey == JOYPAD_RIGHT_THUMB)
	{
		switch (joystick)
		{
		case JOYSTICK_DLEFT:
			return (m_joyKeyState.Gamepad.sThumbRX <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRX <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DRIGHT:
			return (m_joyKeyState.Gamepad.sThumbRX >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRX >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DUP:
			return (m_joyKeyState.Gamepad.sThumbRY >= RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRY >= RIGHT_JOYSTICK_DEADZONE);
			break;
		case JOYSTICK_DDOWN:
			return (m_joyKeyState.Gamepad.sThumbRY <= -RIGHT_JOYSTICK_DEADZONE && m_joyKeyOldState.Gamepad.sThumbRY <= -RIGHT_JOYSTICK_DEADZONE);
			break;
		}
	}
}
