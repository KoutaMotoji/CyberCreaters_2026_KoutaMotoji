//===============================================================================
//
//  C++使った2D(bg.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "t_UI.h"
#include "manager.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CTitleUI::CTitleUI(int nPriority) :CObject2D(nPriority)
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CTitleUI::~CTitleUI()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CTitleUI::Init()
{

}

//==========================================================================================
//終了処理
//==========================================================================================
void CTitleUI::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CTitleUI::Update()
{	
	CObject2D::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CTitleUI::Draw()
{
	CObject2D::Draw();
}

//==========================================================================================
//生成処理
//==========================================================================================
CTitleUI* CTitleUI::Create(D3DXVECTOR3 pos,int Type, D3DXVECTOR2 PolySize)
{
	CTitleUI* background;

	switch (Type)
	{
	case 0:
		background = new CTitleLogo;
		break;
	case 1:
		background = new CTitleButton;
		break;
	default:
		background = new CTitleUI;
		break;
	}
	background->SetPolygonParam(pos, PolySize.y, PolySize.x);
	background->Init();
	return background;
}

//----------------------------------------------------------------------------------------------------------
//UI切り替え用

//==========================================================================================
//初期化オーバーロード
//==========================================================================================
void CTitleLogo::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\title_logo_A.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);
	CObject2D::Init();
}

//==========================================================================================
//更新
//==========================================================================================
void CTitleLogo::Update()
{
	CTitleUI::Update();
}

//==========================================================================================
//初期化
//==========================================================================================
void CTitleButton::Init()
{
	m_TexIdx[0] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\TitleSelect000.png");
	m_TexIdx[1] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\TitleSelect001.png");
	m_TexIdx[2] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\TitleSelect002.png");

	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(m_TexIdx[0]), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);
	CObject2D::Init();
}

//==========================================================================================
//更新
//==========================================================================================
void CTitleButton::Update()
{
	CTitleUI::Update();
}

//==========================================================================================
//生成処理
//==========================================================================================
CTitleButton* CTitleButton::Create(D3DXVECTOR3 pos,D3DXVECTOR2 PolySize)
{
	CTitleButton* background;

	background = new CTitleButton;

	background->SetPolygonParam(pos, PolySize.y, PolySize.x);
	background->Init();
	return background;
}
//==========================================================================================
//テクスチャを変更
//==========================================================================================
void CTitleButton::SetTex(int num)
{
	if (num < 0 || num >= MAX_TEX) {
		return;
	}
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(m_TexIdx[num]), 1, 1);
}

//----------------------------------------------------------------------------------------------------------

//==========================================================================================
//初期化
//==========================================================================================
void CMoveButton::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\skip_over.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);

	CMoveButtonBack::Create(CObject2D::GetPos());
	CObject2D::Init();
	CObject2D::SetColor({ 1.0f,1.0f,1.0f,0.7f });
}

//==========================================================================================
//更新
//==========================================================================================
void CMoveButton::Update()
{
	if (CManager::GetInstance()->GetKeyboard()->GetRepeat(DIK_RETURN) == true ||
		CManager::GetInstance()->GetJoypad()->GetRepeat(CJoypad::JOYPAD_A) == true)
	{
		++m_Gauge;
	}
	if (CManager::GetInstance()->GetKeyboard()->GetRelease(DIK_RETURN) == true && m_Gauge <= PUSHING_TIME ||
		CManager::GetInstance()->GetJoypad()->GetRelease(CJoypad::JOYPAD_A) == true && m_Gauge <= PUSHING_TIME)
	{
		m_Gauge = 0;
	}

	CObject2D::SetGauge(m_Gauge);

	if (m_Gauge > PUSHING_TIME)
	{
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, m_mode);
		return;
	}
	CObject2D::Update();
}


//==========================================================================================
//生成処理
//==========================================================================================
CMoveButton* CMoveButton::Create(CScene::MODE mode)
{
	CMoveButton* background = new CMoveButton;
	D3DXVECTOR3 pos = { SCREEN_WIDTH - PushGaugeWidth * 0.5f,SCREEN_HEIGHT - PushGaugeHeight * 0.5f,0.0f };
	background->SetPolygonParam(pos, PushGaugeHeight, PushGaugeWidth, PUSHING_TIME, false);
	background->Init();
	background->m_mode = mode;
	return background;
}

//==========================================================================================
//初期化
//==========================================================================================
void CMoveButtonBack::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\skip_Under.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);
	CObject2D::Init();
	CObject2D::SetColor({ 1.0f,1.0f,1.0f,0.7f });
}

//==========================================================================================
//生成処理
//==========================================================================================
CMoveButtonBack* CMoveButtonBack::Create(D3DXVECTOR3 pos)
{
	CMoveButtonBack* background = new CMoveButtonBack;

	background->SetPolygonParam(pos, PushGaugeHeight, PushGaugeWidth);
	background->Init();
	return background;
}

//==========================================================================================
//初期化
//==========================================================================================
void CFinishCheck::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\TitleFinish.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);
	CObject2D::Init();
}

//==========================================================================================
//初期化
//==========================================================================================
void CFinishCheck::Update()
{
	CObject2D::SetZoom(m_fSize);
	if (!m_bPush)
	{
		if (m_fSize < 1.0f)
		{
			m_fSize += 0.05f;
		}
	}
	else
	{
		if (m_fSize > 0.0f)
		{
			m_fSize -= 0.05f;
		}
		else
		{
			CObject::Release();
			return;
		}
	}
	if (CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_B))
	{
		m_bPush = true;
	}
	CObject2D::Update();
}

//==========================================================================================
//生成処理
//==========================================================================================
CFinishCheck* CFinishCheck::Create(D3DXVECTOR3 pos)
{
	CFinishCheck* background = new CFinishCheck;

	background->SetPolygonParam(pos, PushGaugeHeight, PushGaugeWidth);
	background->Init();
	return background;
}