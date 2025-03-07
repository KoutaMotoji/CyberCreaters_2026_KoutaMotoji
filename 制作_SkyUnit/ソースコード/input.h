//===============================================================================
//
//		入力処理管理クラス(input.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

#define NUM_KEY_MAX (256)

class CInput
{
public:
	CInput();
	~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;
protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};


#endif // !_INPUT_H_
