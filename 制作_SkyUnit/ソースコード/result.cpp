//===============================================================================
//
//  C++�g����3D(result.cpp)
//								����F���n�O��
// 
//===============================================================================

#include "manager.h"
#include "fade.h"
#include "object.h"
#include "result.h"


#include "r_player.h"
#include "r_boss.h"

#include "eff_smoke.h"
#include "sky_bg.h"
#include "tree_billboard.h"
#include "mesh_cylinder.h"
#include "mesh_obstacle.h"
#include "mesh_ground.h"

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CResult::CResult()
{
	
}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CResult::~CResult()
{

}

//==========================================================================================
//����������
//==========================================================================================
HRESULT CResult::Init()
{
	CScene::Init();
	CResultBG::Create();
	m_pScore = CScore::Create({ SCREEN_WIDTH - 580.0f,SCREEN_HEIGHT - 80.0f,0.0f });
	m_pScore->LoadLastScore();

	m_pGOUI = CGameOverUI::Create({ SCREEN_WIDTH - 250.0f,SCREEN_HEIGHT - 80.0f,0.0f }, m_bSelect);

	CResultPlayer::Create({ 70.0f,530.0f,-170.0f });
	CResultBoss::Create({ -200.0f, 0.0f, 0.0f }, {0.0f,2.0f,0.0f});
	CMeshObstacle::Create({500.0f,-200.0f,3000.0f},4);
	CMeshCylinder::Create({ 0.0f,1000.0f,0.0 });
	CMeshGround::Create({ 500.0f, -200.0f, 0.0f });

	//��������
	std::random_device rnd;				// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
	std::mt19937 mt(rnd());				//  �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
	std::uniform_int_distribution<> rand_x(-5000, 5000);     // [1500, 5000] �͈͂̈�l����
	std::uniform_int_distribution<> rand_z(1000, 4000);		 // [-200, 10000] �͈͂̈�l����

	std::uniform_int_distribution<> type(0, 2);				 // [0, 2] �͈͂̈�l����
	for (int i = 0; i < 100; ++i)
	{
		CTreeBillboard::Create({ (float)rand_x(mt),0.0f,(float)rand_z(mt) }, type(mt));
	}

	CManager::GetInstance()->GetCamera()->SetCamPos({ 10.0f,3000.0f,-300.0 }, { 0.0f,0.0f,0.0f });

	CManager::GetInstance()->GetCamera()->SetFreeCam({ 50.0f,700.0f,-700 }, { 50.0f,500.0f,0.0f }, 160);

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
	return S_OK;
}

//==========================================================================================
//�I������
//==========================================================================================
void CResult::Uninit()
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
//�X�V����
//==========================================================================================
void CResult::Update()
{
	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_RETURN) || CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_A))
	{
		if (!m_bSelect)
		{
			CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_GAME);
		}
		else
		{
			CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_TITLE);
		}
	}
	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_4))
	{
		CManager::GetInstance()->GetCamera()->SetFreeCam({ 0.0f,120.0f,-300 }, { 0.0f,300.0f,500.0f }, 160);
	}
	if (CManager::GetInstance()->GetJoypad()->GetJoyStickTrigger(CJoypad::JOYPAD_LEFT_THUMB, CJoypad::JOYSTICK_DRIGHT) ||
		CManager::GetInstance()->GetJoypad()->GetJoyStickTrigger(CJoypad::JOYPAD_LEFT_THUMB, CJoypad::JOYSTICK_DLEFT) ||
		CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_DPAD_LEFT) ||
		CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_DPAD_RIGHT))
	{
		m_bSelect = !m_bSelect;
		m_pGOUI->ThisDelete();
		m_pGOUI = CGameOverUI::Create({ SCREEN_WIDTH - 250.0f,SCREEN_HEIGHT - 80.0f,0.0f }, m_bSelect);
	}

	CEffSmoke::Create({ -300.0f,-180.0f,300.0f }, 3, 240);
	//CEffSmoke::Create({ 200.0f,-100.0f,600.0f }, 3, 190);
	CEffSmoke::Create({ 500.0f, 0.0f,200.0f }, 3, 150);

	CScene::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CResult::Draw()
{
	CScene::Draw();
}

//==========================================================================================
//�g�̏���������
//==========================================================================================
void CResultBG::Init()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\result.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), 1, 1);

	CObject::SetType(TYPE_2D_UI);
	CObject2D::Init();
}

//==========================================================================================
//�g�̐�������
//==========================================================================================
CResultBG* CResultBG::Create()
{
	CResultBG* bg = new CResultBG;

	bg->SetPolygonParam({SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f}, SCREEN_HEIGHT, SCREEN_WIDTH);
	bg->Init();

	return bg;
}
