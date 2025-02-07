//===============================================================================
//
//  C++使った3D(title.cpp)
//								制作：元地弘汰
// 
//===============================================================================

#include "manager.h"

#include "t_starter.h"
#include "t_base.h"
#include "t_anim_border.h"
#include "t_UI.h"
#include "mesh_cylinder.h"
#include "sky_bg.h"

#include "title.h"
#include "fade.h"
#include "fog.h"

namespace AnimPoint
{
	namespace Anim_Gamestart
	{
		int SECTION_ANIM_START = 1;
		int SECTION_ZOOM_PLAYER = 120;
		int SECTION_LOOK = 90;
		int SECTION_LOOK_ANOTHOR = 260;
		int SECTION_SCENE_CHANGE = 400;
	}
	namespace Anim_Tutorial
	{
		int SECTION_ANIM_START = 1;
		int SECTION_CAM_PULL = 70;
		int SECTION_CAM_ZOOM = 120;
		int SECTION_SCENE_CHANGE = 100;
	}
}

namespace UIState
{
	D3DXVECTOR3 LogoPos = {320.0f,150.0f, 0.0f};
	D3DXVECTOR2 LogoSize = { 600.0f,300.0f };

	D3DXVECTOR3 ButtonPos = {300.0f,500.0f, 0.0f};
	D3DXVECTOR2 ButtonSize = { 550.0f,300.0f };
}

//==========================================================================================
//コンストラクタ
//==========================================================================================
CTitle::CTitle(): m_bNowAnim(false), m_AnimTimer(0), m_nSelect(0)
{
	m_tPl = nullptr;
	m_tSt = nullptr;
	m_tUI = nullptr;
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CTitle::~CTitle()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CTitle::Init()
{
	using namespace UIState;
	CScene::Init();
	CTitleBase::Create({ -400.0f,-120.0f,100.0f });
	m_tPl = CTitlePlayer::Create({ 0.0f, 0.0f, 0.0f});
	m_tSt = CStarter::Create({ 0.0f, -70.0f, 0.0f });
	CMeshCylinder::Create({ 0.0f,1000.0f,0.0f });
	CSkyBg::Create({ 0.0f,-200.0f,0.0f });
	CT_Obj::Create({ -750.0f,-150.0f,-100.0f }, {0.0f,0.0f,0.0f});
	CManager::GetInstance()->GetCamera()->DefuseFreeCam();
	CManager::GetInstance()->GetCamera()->SetCameraHeigjt(600.0f);
	CFog::SetFogLinear(2000.0f, 14000.0f);

	m_tUI = CTitleUI::Create(LogoPos, 0, LogoSize);
	m_tBut = CTitleButton::Create(ButtonPos, ButtonSize);

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CTitle::Uninit()
{
	CFog::FinishFog();
	CScene::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CTitle::Update()
{
	if (!m_bNowAnim)
	{
		CManager::GetInstance()->GetCamera()->AddRotz(0.005f);
		if (CManager::GetInstance()->GetJoypad()->GetJoyStickTrigger(CJoypad::JOYPAD_LEFT_THUMB, CJoypad::JOYSTICK_DUP) ||
			CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_DPAD_UP) ||
			CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_W))
		{
			--m_nSelect;
			if (m_nSelect < SELECT_GAME)
			{
				m_nSelect = SELECT_FINISH;
			}
			m_tBut->SetTex(m_nSelect);
		}
		else if (CManager::GetInstance()->GetJoypad()->GetJoyStickTrigger(CJoypad::JOYPAD_LEFT_THUMB, CJoypad::JOYSTICK_DDOWN) ||
			CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_DPAD_DOWN) ||
			CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_S))
		{
			++m_nSelect;
			if (m_nSelect > SELECT_FINISH)
			{
				m_nSelect = SELECT_GAME;
			}
			m_tBut->SetTex(m_nSelect);
		}
		if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_RETURN)||
			CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_A) == true)
		{
			m_AnimTimer = 0;
			m_bNowAnim = true;
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_TITLESE_ENTER);
		}
	}
	else
	{
		UpdateAnim(m_nSelect);
	}
	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_O))
	{
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_TUTORIAL);
	}
	CScene::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CTitle::Draw()
{
	CScene::Draw();
}

//==========================================================================================
//タイトルアニメーション処理
//==========================================================================================
void CTitle::UpdateAnim(int m)
{

	++m_AnimTimer;
	//設定されたタイムテーブル通りにアニメーションさせる処理
	if (m == SELECT_GAME)
	{
		using namespace AnimPoint::Anim_Gamestart;

		if (m_AnimTimer == SECTION_ANIM_START)
		{
			CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f, 0.0f, -300.0f }, { 0.0f, 30.0f, 0.0f }, SECTION_ZOOM_PLAYER);
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_TITLESE_OPEN);
			m_tSt->SetStart();
			CMoveButton::Create(CScene::MODE_GAME);
			CTAnimBorder::Create(0);
			CTAnimBorder::Create(1);
			m_tBut->Release();
			m_tUI->Destroy();
		}
		if (m_AnimTimer == SECTION_LOOK_ANOTHOR)
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_TITLESE_MOVE);
			CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f, 60.0f, 160.0f }, { 0.0f, 10.0f, -500.0f }, SECTION_LOOK);
		}
		if (m_AnimTimer > SECTION_LOOK_ANOTHOR + SECTION_LOOK)
		{
			m_tPl->AddMove({ 0.0f,0.0f,-0.45f });
		}
		if (m_AnimTimer == SECTION_SCENE_CHANGE)
		{
			CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_GAME);
		}
	}
	//設定されたタイムテーブル通りにアニメーションさせる処理
	else if (m == SELECT_TUTORIAL)
	{
		using namespace AnimPoint::Anim_Tutorial;

		if (m_AnimTimer == SECTION_ANIM_START)
		{
			CManager::GetInstance()->GetCamera()->SetFreeCam({ -750.0f,100.0f, -2000.0f }, { -750.0f, -100.0f, 0.0f }, SECTION_CAM_PULL);
			CMoveButton::Create(CScene::MODE_TUTORIAL);
			CTAnimBorder::Create(0);
			CTAnimBorder::Create(1);
			m_tBut->Release();
			m_tUI->Destroy();

		}
		if (m_AnimTimer == SECTION_CAM_PULL)
		{
			CManager::GetInstance()->GetCamera()->SetFreeCam({ -750.0f, -100.0f, 0.0f }, { -750.0f, -100.0f, 0.0f }, SECTION_CAM_ZOOM);
		}
		if (m_AnimTimer == SECTION_SCENE_CHANGE)
		{
			CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_TUTORIAL);
		}
	}
	else
	{
		if (m_AnimTimer == 1)
		{
			CFinishCheck::Create({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f });
		}
		if (CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_A))
		{
			SetExitGame();
			return;
		}
		else if (CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_B))
		{
			m_bNowAnim = false;
		}

		m_AnimTimer = -1;
	}
}

CT_Obj* CT_Obj::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CT_Obj* obj = new CT_Obj();

	obj->BindModel("data\\MODEL\\warehouse.x");
	obj->SetModelParam(pos);
	obj->SetRot(rot);

	return obj;
}