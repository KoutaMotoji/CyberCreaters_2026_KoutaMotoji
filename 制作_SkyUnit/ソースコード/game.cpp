//===============================================================================
//
//  C++�g����3D(game.cpp)
//								����F���n�O��
// 
//===============================================================================

#include "manager.h"
#include "fade.h"
#include "object.h"
#include "sky_bg.h"

#include "mesh_Boss_Terra.h"
#include "mesh_cylinder.h"
#include "tree_billboard.h"
#include "boss_reticle.h"

#include "fog.h"
#include "game.h"
#include "map_edit.h"
#include "player_observer.h"
#include "mesh_ground.h"

#include "playerX.h"


//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CGame::CGame()
{
	m_Score = nullptr;
}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CGame::~CGame()
{

}

//==========================================================================================
//����������
//==========================================================================================
HRESULT CGame::Init()
{
	CScene::Init();

	CFog::SetFogLinear(4500.0f,15000.0f);
	CPlayerX::Create({ 0.0f,0.0f,0.0f });
	CPlayerObserver::GetInstance()->PlayerSearch();

	CMeshCylinder::Create({ 0.0f,1000.0f,0.0f });
	CMeshGround::Create({ 0.0f,-1000.0f,0.0f });
	CMeshGround::Create({ 0.0f,-1000.0f,5940 * 2 });
	CMeshGround::Create({ 0.0f,-1000.0f,5940 * 4 });
	m_Score = CScore::Create();
	m_Score->ResetScore();
	//m_Score->LoadLastScore();
	CMapEdit::SetLoadMap();
	MakeRandTree();

	CManager::GetInstance()->GetCamera()->SetCameraHeigjt(50.0f);
	CManager::GetInstance()->GetCamera()->DefuseFreeCam();

	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
	return S_OK;
}

//==========================================================================================
//�I������
//==========================================================================================
void CGame::Uninit()
{
	CFog::FinishFog();
	CPlayerObserver::GetInstance()->PlayerObsDestroy();
	if (m_Score != nullptr)
	{
		m_Score->Uninit();
		delete m_Score;
		m_Score = nullptr;
	}
	CScene::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CGame::Update()
{
#if _DEBUG

	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_RESULT);
	}
	if (CManager::GetInstance()->GetKeyboard()->CKeyboard::GetTrigger(DIK_F3))
	{
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_MAPEDIT);
		return;
	}
#endif // _DEBUG
	CScene::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CGame::Draw()
{
	CScene::Draw();
}

//==========================================================================================
//�i�F�p�̖؂������_���Őݒu���鏈��
//==========================================================================================
void CGame::MakeRandTree()
{
	//��������
	std::random_device rnd;				// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
	std::mt19937 mt(rnd());				//  �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
	std::uniform_int_distribution<> rand_x(1500, 5000);     // [1500, 5000] �͈͂̈�l����
	std::uniform_int_distribution<> rand_z(-200, 10000);		 // [-200, 10000] �͈͂̈�l����

	std::uniform_int_distribution<> type(0, 2);				 // [0, 2] �͈͂̈�l����
	for (int i = 0; i < MAX_TREESET_NUM * 0.5; ++i)
	{
		CTreeBillboard::Create({ (float)rand_x(mt),0.0f,(float)rand_z(mt) }, type(mt));
	}
	for (int i = 0; i < MAX_TREESET_NUM * 0.5; ++i)
	{

		CTreeBillboard::Create({ (float)rand_x(mt) * -1,0.0f,(float)rand_z(mt) }, type(mt));
	}
}