//===============================================================================
//
//  C++�g����2D(mesh_obstacle.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "mesh_obstacle.h"
#include "player_observer.h"
#include "mesh_Boss_Terra.h"
#include "enemy_base.h"
#include "3D_Item.h"

#include "manager.h"
#include "game.h"

namespace {
	enum
	{
		OBS_TYPE_GATE = 0,
		OBS_TYPE_STATUE,
		OBS_TYPE_BUILDING0,
		OBS_TYPE_BUILDING1,
		OBS_TYPE_TOWER,
		OBS_TYPE_BRIGHE,
		OBS_TYPE_PAIL,
		OBS_TYPE_BOSS_SPAWNER,
		OBS_TYPE_ITEM_EMITTER,
		OBS_TYPE_ENEMY_SPAWNER,
		OBS_TYPE_GUIDE_SIDE,
		OBS_TYPE_GUIDE_OVER,

	};
	const char* modelName[MAX_MODELTYPE]{
		"data\\MODEL\\gate000.x",
		"data\\MODEL\\statue000.x",
		"data\\MODEL\\Building000.x",
		"data\\MODEL\\Building001.x",	
		"data\\MODEL\\tower.x",
		"data\\MODEL\\brighe.x",
		"data\\MODEL\\pail.x",
		"data\\MODEL\\boss_spawner.x",
		"data\\MODEL\\ItemEmitter.x",
		"data\\MODEL\\enemy_spawner.x",
		"data\\MODEL\\guide_side.x",
		"data\\MODEL\\guide_over.x",

	};
};

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CMeshObstacle::CMeshObstacle(int nPriority) :CObjectX(nPriority)
{

}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CMeshObstacle::~CMeshObstacle()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CMeshObstacle::Init()
{
	CObject::SetType(TYPE_3D_OBSTACLE);
	CObjectX::Init();
}

//==========================================================================================
//�I������
//==========================================================================================
void CMeshObstacle::Uninit()
{
	CObjectX::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CMeshObstacle::Update()
{
	if (PlayerDistanceCheck())
	{
		Release();
		return;
	}
	CObjectX::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CMeshObstacle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	// �@���̎������K����L����
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	if (CPlayerObserver::GetInstance()->GetPlayerPos().z < CObjectX::GetPos().z + 10000)
	{
		CObjectX::Draw();
	}
	// �@���̎������K���𖳌���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

//==========================================================================================
//�`�揈��
//==========================================================================================
bool CMeshObstacle::PlayerDistanceCheck()
{
	if (CManager::GetInstance()->GetScene()->GetSceneMode() == CScene::MODE_GAME)
	{
		return (CObjectX::GetPos().z - CPlayerObserver::GetInstance()->GetPlayerPos().z < -OBJ_DESTROY_DIS);
	}
	return false;
}

//==========================================================================================
//��������
//==========================================================================================
CMeshObstacle* CMeshObstacle::Create(D3DXVECTOR3 pos,int Type)
{
	CMeshObstacle* enemy;

	switch (Type)
	{
	case 7:
		enemy = new CBossEmitter;
		break;
	case 8:
		enemy = new CScoreItemEmitter;
		break;
	case 9:
		enemy = new CEnemyEmitter;
		break;
	default:
		enemy = new CMeshObstacle;
		break;
	}

	enemy->BindModel(modelName[Type]);

	enemy->SetModelParam(pos);
	enemy->Init();

	return enemy;
}

//==========================================================================================
//��������(�z�u�c�[���p�I�[�o�[���[�h)
//==========================================================================================
CMeshObstacle* CMeshObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int Type)
{
	CMeshObstacle* enemy;

	switch (Type)
	{
	case OBS_TYPE_BOSS_SPAWNER:
		enemy = new CBossEmitter;
		break;
	case OBS_TYPE_ITEM_EMITTER:
		enemy = new CScoreItemEmitter;
		break;
	case OBS_TYPE_ENEMY_SPAWNER:
		enemy = new CEnemyEmitter;
		break;
	default:
		enemy = new CMeshObstacle;
		break;
	}
	enemy->BindModel(modelName[Type]);

	enemy->SetModelParam(pos);
	enemy->SetRot(rot);
	enemy->SetSize(scale);
	enemy->Init();

	return enemy;
}

//---------------------------------------------------------------------------------------------------------------------------------
//����I�u�W�F�z�u�p�ݒ�

//�������I�[�o�[���[�h
void CScoreItemEmitter::Init()
{
	if (CManager::GetInstance()->GetScene()->GetSceneMode() == CScene::MODE_GAME)
	{
		m_bSceneCheck = true;
	}
}

//�X�V�I�[�o�[���[�h
void CScoreItemEmitter::Update()
{
	if (m_bSceneCheck)
	{
		C3DItem::Create(CObjectX::GetPos());
		Release();
		return;
	}
	CMeshObstacle::Update();
}

//�������I�[�o�[���[�h
void CBossEmitter::Init()
{
	if (CManager::GetInstance()->GetScene()->GetSceneMode() == CScene::MODE_GAME)
	{
		m_bSceneCheck = true;
	}
}

//�X�V�I�[�o�[���[�h
void CBossEmitter::Update()
{
	if (m_bSceneCheck)
	{
		if (PlayerLength())
		{
			CBossTerra::Create(CObjectX::GetPos());

			Release();
			return;
		}
	}
	CMeshObstacle::Update();
}

bool CBossEmitter::PlayerLength()
{
	return (CObjectX::GetPos().z - CPlayerObserver::GetInstance()->GetPlayerPos().z < BOSS_EMITTE_DIS);
}

//�������I�[�o�[���[�h
void CEnemyEmitter::Init()
{
	if (CManager::GetInstance()->GetScene()->GetSceneMode() == CScene::MODE_GAME)
	{
		m_bSceneCheck = true;
	}
}

//�X�V�I�[�o�[���[�h
void CEnemyEmitter::Update()
{
	if (m_bSceneCheck)
	{
		if (PlayerLength())
		{
			CEnemyBase::Create(CObjectX::GetPos());

			Release();
			return;
		}
	}
	CMeshObstacle::Update();
}

bool CEnemyEmitter::PlayerLength()
{
	return (CObjectX::GetPos().z - CPlayerObserver::GetInstance()->GetPlayerPos().z < ENEMY_EMITTE_DIS);
}
//---------------------------------------------------------------------------------------------------------------------------------