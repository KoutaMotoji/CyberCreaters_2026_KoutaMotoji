//===============================================================================
//
//  プレイヤーのUI表示(player_UI.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "player_UI.h"

#include "player_observer.h"
#include "manager.h"

//静的メンバ初期化
const float CGaugeLife::OBJ_HEIGHT = 45.0f;
const float CGaugeLife::OBJ_WIDTH = SCREEN_WIDTH * 0.3f;

namespace MainUIState
{
	D3DXVECTOR3 UIpos = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
}

//==========================================================================================
//コンストラクタ
//==========================================================================================
CGaugeLife::CGaugeLife(int nPriority) :CObject2D(nPriority)
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CGaugeLife::~CGaugeLife()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CGaugeLife::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\hp_bar.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CGaugeLiFrame::Create(CObject2D::GetPos(), OBJ_WIDTH, OBJ_HEIGHT);
	CGaugeLiBack::Create(CObject2D::GetPos(), OBJ_WIDTH, OBJ_HEIGHT);

	CObject::SetType(TYPE_2D_UI);
	CObject2D::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CGaugeLife::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CGaugeLife::Update()
{
	CGaugeLife::SetGauge();
	CObject2D::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CGaugeLife::Draw()
{
	CObject2D::Draw();
}

//==========================================================================================
//生成処理
//==========================================================================================
CGaugeLife* CGaugeLife::Create(int nMaxGauge)
{
	CGaugeLife* gauge = new CGaugeLife;
	D3DXVECTOR3 pos = { OBJ_WIDTH * 0.5f,OBJ_HEIGHT * 0.5f + 25.0f,0.0f };
	gauge->SetPolygonParam(pos, OBJ_HEIGHT, OBJ_WIDTH, nMaxGauge, false);
	gauge->Init();
	return gauge;
}

//==========================================================================================
//数値を取得して反映
//==========================================================================================
void CGaugeLife::SetGauge()
{
	int nNowGauge = CPlayerObserver::GetInstance()->GetPlayerLife();
	CObject2D::SetGauge(nNowGauge);
}

//----------------------------------------------------------------------------------------------------------------------------------
//枠・後ろ設定用

//==========================================================================================
//枠の初期化処理
//==========================================================================================
void CGaugeLiFrame::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\hp_frame.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_2D_UI);
	CObject2D::Init();
}


//==========================================================================================
//枠の生成処理
//==========================================================================================
CGaugeLiFrame* CGaugeLiFrame::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CGaugeLiFrame* gauge = new CGaugeLiFrame;
	gauge->SetPolygonParam(pos, fHeight, fWidth);
	gauge->Init();
	return gauge;
}

//==========================================================================================
//後ろ側の初期化処理
//==========================================================================================
void CGaugeLiBack::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\hp_back.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_2D_UI);
	CObject2D::Init();
}

//==========================================================================================
//後ろ側の生成処理
//==========================================================================================
CGaugeLiBack* CGaugeLiBack::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CGaugeLiBack* gauge = new CGaugeLiBack;
	gauge->SetPolygonParam(pos, fHeight, fWidth, { 0.2f,0.4f,0.9f,1.0f });
	gauge->Init();
	return gauge;
}


//==========================================================================================
//メインUIの初期化処理
//==========================================================================================
void CMainUI::Init()
{
	m_TexIdx[0] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\mainUI_B.png");
	m_TexIdx[1] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\mainUI_A.png");

	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(m_TexIdx[0]), 1, 1);

	CObject::SetType(TYPE_2D_UI);
	CObject2D::Init();
}

//==========================================================================================
//メインUIの生成処理
//==========================================================================================
CMainUI* CMainUI::Create()
{
	using namespace MainUIState;
	CMainUI* UI = new CMainUI;
	UI->SetPolygonParam(UIpos, SCREEN_HEIGHT, SCREEN_WIDTH);
	UI->Init();
	return UI;
}

//==========================================================================================
//メインUIの切り替え処理
//==========================================================================================
void CMainUI::ChangeUI(int playermode)
{
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(m_TexIdx[playermode]), 1, 1);
}