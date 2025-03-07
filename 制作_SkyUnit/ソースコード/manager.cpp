//===============================================================================
//
//  各管理クラスの一括管理クラス(manager.cpp)
//								制作：元地弘汰
// 
//===============================================================================

#include "manager.h"
#include <stdlib.h>
#include <stdio.h>
#include "object.h"
#include "object2D.h"
#include "object3D.h"

CManager* CManager::_instance = nullptr;

//==========================================================================================
//コンストラクタ
//==========================================================================================
CManager::CManager():MAP_SLICE_X(24), MAP_SLICE_Y(18)
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CManager::~CManager()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラーの生成
	m_pRenderer = new CRenderer();
	//カメラの生成
	m_pCamera = new CCamera();
	//ライトの生成
	m_pLight = new CLight();
	//サウンドの生成
	m_pSound = new CSound();

	//キーボードの生成
	m_pKeyboard = new CKeyboard();
	//ジョイパッドの生成
	m_pJoypad = new CJoypad();

	//テクスチャの生成
	m_pTexture = new CTexture();

	m_pFade = new CFade();
	//初期化設定
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{//初期化が失敗した場合
		return -1;
	}
	//キーボードの初期化設定
	m_pKeyboard->Init(hInstance, hWnd);
	//ジョイパッドの初期化
	m_pJoypad->Init(hInstance,hWnd);
	//カメラの初期化
	m_pCamera->Init();
	//サウンドの初期化
	m_pSound->Init(hWnd);
	//ライトの初期化
	m_pLight->Init();

	SetMode(CScene::MODE_TITLE);
	m_pFade->SetFade(m_pFade->FADE_OUT, CScene::MODE_GAME);
	return S_OK;
}

//==========================================================================================
//終了商品
//==========================================================================================
void CManager::Uninit()
{
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
	if (m_pTexture != nullptr)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_pJoypad != nullptr)
	{
		m_pJoypad->Uninit();
		delete m_pJoypad;
		m_pJoypad = nullptr;
	}
	if (m_pKeyboard != nullptr)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CManager::Update()
{

	m_pFade->Update();
	
	m_pScene->Update();
	m_pRenderer->Update();
	m_pKeyboard->Update();
	m_pJoypad->Update();
	m_pCamera->Update();
	
}

//==========================================================================================
//描画処理
//==========================================================================================
void CManager::Draw()
{
	m_pRenderer->Draw(m_pScene);
}

//==========================================================================================
//シーン切り替え
//==========================================================================================
void CManager::SetMode(CScene::MODE mode)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}
	m_pScene = CScene::Create(mode);
	m_pScene->Init();
}
