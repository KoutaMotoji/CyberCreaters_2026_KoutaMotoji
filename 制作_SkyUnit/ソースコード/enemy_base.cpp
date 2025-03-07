//===============================================================================
//
//  �G���N���X(���݌p�����Ă���N���X�͑��݂��܂���)(enemy_base.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "enemy_base.h"
#include "eff_explosion.h"
#include "player_observer.h"

#include "manager.h"
#include "game.h"


//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CEnemyBase::CEnemyBase() :m_bMove(false), m_DefPos({0.0f,0.0f,0.0f}), m_bLockOned(false), lockon(nullptr)
{
	m_pShadow = nullptr;
}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CEnemyBase::~CEnemyBase()
{
	m_DefPos = CObjectX::GetPos();
}

//==========================================================================================
//����������
//==========================================================================================
void CEnemyBase::Init()
{
	CObject::SetType(TYPE_3D_ENEMY);
	m_pShadow = CShadow::Create({ 0.0f,0.0f,0.0f });
	CObjectX::Init();
}

//==========================================================================================
//�I������
//==========================================================================================
void CEnemyBase::Uninit()
{

	CObjectX::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CEnemyBase::Update()
{
	D3DXVECTOR3 pos = CObjectX::GetPos();
	m_pShadow->SetShadowGround(pos);

	if (pos.y >m_DefPos.y +  300.0f ||
		pos.y < m_DefPos .y - 300.0f)
	{
		m_bMove = !m_bMove;
	}
	
	if (m_bMove)
	{
		CObjectX::AddPos({ 0.0f,3.0f,0.0f });
	}
	else
	{
		CObjectX::AddPos({ 0.0f,-3.0f,0.0f });
	}
	if (m_bLockOned)
	{
		lockon->SetThisPos(CObjectX::GetPos());
	}
	if (CObjectX::GetPos().z - CPlayerObserver::GetInstance()->GetPlayerPos().z < -1000)
	{
		Release();
		return;
	}
	CObjectX::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CEnemyBase::Draw()
{
	if (m_bLockOned)
	{
		if (lockon != nullptr)
		{
			lockon->Draw();
		}
	}
	CObjectX::Draw();
}

//==========================================================================================
//��������
//==========================================================================================
CEnemyBase* CEnemyBase::Create(D3DXVECTOR3 pos)
{
	CEnemyBase* enemy = new CEnemyBase;

	enemy->BindModel("data\\MODEL\\enemy_base.x");
	enemy->SetModelParam(pos);
	enemy->Init();

	return enemy;
}

//==========================================================================================
//��������(�`���[�g���A���p)
//==========================================================================================
CEnemyBase* CEnemyBase::Create(D3DXVECTOR3 pos,float size)
{
	CEnemyBase* enemy = new CEnemyBase;

	enemy->BindModel("data\\MODEL\\target.x");
	enemy->SetModelParam(pos);
	enemy->Init();
	enemy->SetSize({ size ,size ,size });
	return enemy;
}

void CEnemyBase::Damaged()
{
	CEffExplosion::Create(CObjectX::GetPos(), 300.0f);
	if (lockon != nullptr)
	{
		lockon->Release();
	}
	if (m_pShadow != nullptr)
	{
		m_pShadow->Release();
	}
	CScene* pScene = CManager::GetInstance()->GetScene();
	if (pScene->GetSceneMode() == CScene::MODE_GAME)
	{
		CGame* pGame = dynamic_cast<CGame*>(pScene);
		pGame->GetScore()->AddScore(200);
	}
	Release();
	return;
}