//===============================================================================
//
//		C++égÇ¡ÇΩ2D(input.h)
//								êßçÏÅFå≥ínçOëø
// 
//===============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "input.h"

class CKeyboard : public CInput
{
public:
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd)override;
	void Uninit()override;
	void Update()override;

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];

	static LPDIRECTINPUTDEVICE8 m_pDevKeyboard;
};

#endif