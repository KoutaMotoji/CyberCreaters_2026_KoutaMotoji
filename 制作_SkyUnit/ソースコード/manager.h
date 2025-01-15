//===============================================================================
//
//  C++使った2D(manager.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

#include "renderer.h"
#include "texture.h"
#include "keyboard.h"
#include "joypad.h"
#include "camera.h"
#include "sound.h"
#include "light.h"
#include "scene.h"
#include "fade.h"

class CManager
{
private:
	CManager();
	static CManager* _instance;

public:
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	static CManager* GetInstance()	//シングルトン用インスタンス
	{
		if (_instance == nullptr)
		{//初回用生成処理
			_instance = new CManager;
		}
		return _instance;
	}
	CRenderer* GetRenderer() { return m_pRenderer; };
	CKeyboard* GetKeyboard() { return m_pKeyboard; };
	CTexture* GetTexture() { return m_pTexture; };
	CJoypad* GetJoypad() { return m_pJoypad; };
	CCamera* GetCamera() { return m_pCamera; };
	CSound* GetSound() { return m_pSound; };
	CLight* GetLight() { return m_pLight; };
	CScene* GetScene() { return m_pScene; };
	CFade* GetFade() { return m_pFade; };
	 void SetMode(CScene::MODE mode);
	 const int MAP_SLICE_X;
	 const int MAP_SLICE_Y;
private:
	 CRenderer* m_pRenderer;
	 CKeyboard* m_pKeyboard;
	 CTexture* m_pTexture;
	 CJoypad* m_pJoypad;
	 CCamera* m_pCamera;
	 CSound* m_pSound;
	 CLight* m_pLight;
	 CScene* m_pScene;
	 CFade* m_pFade;
};

#endif