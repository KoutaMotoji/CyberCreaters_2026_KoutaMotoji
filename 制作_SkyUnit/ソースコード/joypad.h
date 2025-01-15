//===============================================================================
//
//		C++使った2D(input.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _JOYPAD_H_
#define _JOYPAD_H_

#include "input.h"

class CJoypad : public CInput
{
public:
	typedef enum
	{
		JOYPAD_DPAD_UP = 0,
		JOYPAD_DPAD_DOWN,
		JOYPAD_DPAD_LEFT,
		JOYPAD_DPAD_RIGHT,
		JOYPAD_START,
		JOYPAD_BACK,
		JOYPAD_LEFT_THUMB,
		JOYPAD_RIGHT_THUMB,
		JOYPAD_LEFT_SHOULDER,
		JOYPAD_RIGHT_SHOULDER,
		JOYPAD_RIGHT_TRIGGER,
		JOYPAD_LEFT_TRIGGER,
		JOYPAD_A,
		JOYPAD_B,
		JOYPAD_X,
		JOYPAD_Y,
		JOYPAD_MAX
	}JOYKEY;

	typedef enum
	{
		JOYSTICK_DLEFT = 0,
		JOYSTICK_DRIGHT,
		JOYSTICK_DUP,
		JOYSTICK_DDOWN
	}JOYSTICK;

	CJoypad();
	~CJoypad();
	HRESULT Init(HINSTANCE hInstace, HWND hWnd)override;

	void Uninit()override;
	void Update()override;

	bool GetPress(JOYKEY key);		//キーのプレス情報
	bool GetTrigger(JOYKEY key);	//キーのトリガー情報
	bool GetRelease(JOYKEY key);	//キーのリリース情報
	bool GetRepeat(JOYKEY key);		//キーのリピート情報

	bool GetJoyStickL(JOYSTICK joystick);						//左スティックの入力情報
	bool GetJoyStickR(JOYSTICK joystick);						//右スティックの入力情報
	bool GetJoyStickTrigger(JOYKEY joykey, JOYSTICK joystick);	//スティックのトリガー情報
	bool GetJoyStickRelease(JOYKEY joykey, JOYSTICK joystick);	//スティックのリリース情報
	bool GetJoyStickRepeat(JOYKEY joykey, JOYSTICK joystick);	//スティックのリピート情報

	D3DXVECTOR2 GetJoyStickVecL();
	D3DXVECTOR2 GetJoyStickVecR();

private:

	XINPUT_STATE m_joyKeyState;				//キーの入力情報
	XINPUT_STATE m_joyKeyOldState;			//キーの過去の入力情報
	XINPUT_STATE m_joyKeyStateTrigger;		//キーのトリガー判定情報
	XINPUT_STATE m_joyKeyStateRelease;		//キーのリリース判定情報
	XINPUT_STATE m_joyKeyStateRepeat;		//キーのリピート判定情報
};



#endif