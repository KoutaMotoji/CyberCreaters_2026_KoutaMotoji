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
	int SECTION_ZOOM_PLAYER = 120;
	int SECTION_LOOK = 90;
	int SECTION_LOOK_ANOTHOR = 260;
	int SECTION_SCENE_CHANGE = 400;
}

namespace UIState
{
	D3DXVECTOR3 LogoPos = {320.0f,150.0f, 0.0f};
	D3DXVECTOR2 LogoSize = { 600.0f,300.0f };

	D3DXVECTOR3 ButtonPos = {SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT - 120.0f, 0.0f};
	D3DXVECTOR2 ButtonSize = { 400.0f,100.0f };
}

//==========================================================================================
//コンストラクタ
//==========================================================================================
CTitle::CTitle(): m_bNowAnim(false), m_AnimTimer(0)
{
	m_tPl = nullptr;
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
	CStarter::Create({ 0.0f, -70.0f, 0.0f });
	CMeshCylinder::Create({ 0.0f,1000.0f,0.0f });
	CSkyBg::Create({ 0.0f,-200.0f,0.0f });

	CManager::GetInstance()->GetCamera()->SetCameraHeigjt(600.0f);
	CFog::SetFogLinear(2000.0f, 14000.0f);

	CTitleUI::Create(LogoPos, 0, LogoSize);
	CTitleUI::Create(ButtonPos, 1, ButtonSize);

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
		if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_RETURN)||
			CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_A) == true)
		{
			CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f, 0.0f, -300.0f }, { 0.0f, 30.0f, 0.0f }, AnimPoint::SECTION_ZOOM_PLAYER);
			m_bNowAnim = true;
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_TITLESE_ENTER);
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_TITLESE_OPEN);

			CTAnimBorder::Create(0);
			CTAnimBorder::Create(1);
			CMoveButton::Create();
		}
	}
	else
	{
		UpdateAnim();
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
void CTitle::UpdateAnim()
{
	using namespace AnimPoint;

	++m_AnimTimer;

	if (m_AnimTimer == SECTION_LOOK_ANOTHOR)
	{
		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_TITLESE_MOVE);
		CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f, 60.0f, 160.0f }, { 0.0f, 10.0f, -500.0f }, SECTION_LOOK);
	}
	if (m_AnimTimer > SECTION_LOOK_ANOTHOR + SECTION_LOOK)
	{
		m_tPl->AddMove({ 0.0f,0.0f,-0.45f });
	}
	if(m_AnimTimer == SECTION_SCENE_CHANGE)
	{
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_GAME);
	}
}