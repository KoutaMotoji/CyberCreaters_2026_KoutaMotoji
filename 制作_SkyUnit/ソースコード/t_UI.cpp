//===============================================================================
//
//  C++�g����2D(bg.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "t_UI.h"
#include "manager.h"

//�ÓI�����o������

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CTitleUI::CTitleUI(int nPriority) :CObject2D(nPriority)
{

}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CTitleUI::~CTitleUI()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CTitleUI::Init()
{

}

//==========================================================================================
//�I������
//==========================================================================================
void CTitleUI::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CTitleUI::Update()
{
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_A) == true)
	{
		Release();
		return;
	}
	CObject2D::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CTitleUI::Draw()
{
	CObject2D::Draw();
}

//==========================================================================================
//��������
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
//UI�؂�ւ��p

//�������I�[�o�[���[�h
void CTitleLogo::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\title_logo.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);
	CObject2D::Init();
}

//�X�V�I�[�o�[���[�h
void CTitleLogo::Update()
{
	CTitleUI::Update();
}

//�������I�[�o�[���[�h
void CTitleButton::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\title_ui.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);
	CObject2D::Init();
}

//�X�V�I�[�o�[���[�h
void CTitleButton::Update()
{
	AlphableUI();
	CTitleUI::Update();
}

void CTitleButton::AlphableUI()
{
	float speed = SWAP_SPEED;

	if (m_LocalCol.a <= 0.0f ||
		m_LocalCol.a >= 1.0f)
	{
		m_bColSwitch = m_bColSwitch ? false : true;
	}
	if (m_bColSwitch)
	{
		m_LocalCol.a -= speed;
	}
	else
	{
		m_LocalCol.a += speed;
	}
	CObject2D::SetColor(m_LocalCol);
}
//----------------------------------------------------------------------------------------------------------

//�������I�[�o�[���[�h
void CMoveButton::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\skip_over.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);

	CMoveButtonBack::Create(CObject2D::GetPos());
	CObject2D::Init();
	CObject2D::SetColor({ 1.0f,1.0f,1.0f,0.7f });
}

//�X�V�I�[�o�[���[�h
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
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_GAME);
		return;
	}
	CObject2D::Update();
}


//==========================================================================================
//��������
//==========================================================================================
CMoveButton* CMoveButton::Create()
{
	CMoveButton* background = new CMoveButton;
	D3DXVECTOR3 pos = { SCREEN_WIDTH - PushGaugeWidth * 0.5f,SCREEN_HEIGHT - PushGaugeHeight * 0.5f,0.0f };
	background->SetPolygonParam(pos, PushGaugeHeight, PushGaugeWidth, PUSHING_TIME, false);
	background->Init();
	return background;
}

//�������I�[�o�[���[�h
void CMoveButtonBack::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\skip_Under.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_3D_TITLEOBJ);
	CObject2D::Init();
	CObject2D::SetColor({ 1.0f,1.0f,1.0f,0.7f });
}

//==========================================================================================
//��������
//==========================================================================================
CMoveButtonBack* CMoveButtonBack::Create(D3DXVECTOR3 pos)
{
	CMoveButtonBack* background = new CMoveButtonBack;

	background->SetPolygonParam(pos, PushGaugeHeight, PushGaugeWidth);
	background->Init();
	return background;
}