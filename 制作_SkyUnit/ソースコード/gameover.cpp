//===============================================================================
//
//  C++使った3D(result.cpp)
//								制作：元地弘汰
// 
//===============================================================================

#include "manager.h"
#include "fade.h"
#include "object.h"
#include "gameover.h"
#include "player_observer.h"

#include "go_player.h"
#include "eff_smoke.h"
#include "sky_bg.h"
#include "mesh_cylinder.h"
#include "mesh_ground.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CGameover::CGameover()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CGameover::~CGameover()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CGameover::Init()
{
	CScene::Init();
	CGameoverBG::Create();
	m_pScore = CScore::Create({ SCREEN_WIDTH - 80.0f,SCREEN_HEIGHT - 80.0f,0.0f });
	m_pScore->LoadLastScore();
	CGameOverPlayer::Create({ 0.0f, -150.0f, 0.0f });
	CMeshCylinder::Create({ 0.0f,1000.0f,0.0 });

	CMeshGround::Create({500.0f, -200.0f, 0.0f});
	CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f,50.0f,-300 }, { 0.0f,-100.0f,0.0f }, 150);

	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CGameover::Uninit()
{
	if (m_pScore != nullptr)
	{
		m_pScore->Uninit();
		delete m_pScore;
		m_pScore = nullptr;
	}
	CScene::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CGameover::Update()
{
	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_RETURN) || CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_A))
	{
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_TITLE);
	}
	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_4))
	{
		CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f,100.0f,-2000 }, { 0.0f,-200.0f,0.0f }, 150);
	}
	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_5))
	{
		CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f,100.0f,-200 }, { 0.0f,-100.0f,0.0f }, 150);
	}
	CEffSmoke::Create({ 0.0f,-100.0f,0.0f }, 3, 240);
	CScene::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CGameover::Draw()
{
	CScene::Draw();
}

//==========================================================================================
//枠の初期化処理
//==========================================================================================
void CGameoverBG::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\gameover.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_2D_UI);
	CObject2D::Init();
}


//==========================================================================================
//枠の生成処理
//==========================================================================================
CGameoverBG* CGameoverBG::Create()
{
	CGameoverBG* bg = new CGameoverBG;

	bg->SetPolygonParam({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f }, SCREEN_HEIGHT, SCREEN_WIDTH);
	bg->Init();

	return bg;
}
