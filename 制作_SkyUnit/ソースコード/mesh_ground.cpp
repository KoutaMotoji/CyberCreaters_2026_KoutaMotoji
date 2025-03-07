//===============================================================================
//
//  �n�`���b�V���̏���(mesh_ground.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "mesh_ground.h"
#include "player_observer.h"

#include "manager.h"
#include "game.h"

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CMeshGround::CMeshGround(int nPriority) :CObjectX(nPriority), m_mode(0)
{

}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CMeshGround::~CMeshGround()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CMeshGround::Init()
{
	CObject::SetType(TYPE_3D_MESHOBJECT);
	CObjectX::Init();
}

//==========================================================================================
//�I������
//==========================================================================================
void CMeshGround::Uninit()
{
	CObjectX::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CMeshGround::Update()
{
	if (CManager::GetInstance()->GetScene()->GetSceneMode() == CScene::MODE_GAME ||
		CManager::GetInstance()->GetScene()->GetSceneMode() == CScene::MODE_TUTORIAL)
	{
		if (CPlayerObserver::GetInstance()->GetPlayerPos().z - this->GetPos().z > this->GetModelMax().z * 1.5f)
		{
			D3DXVECTOR3 newPos = CObjectX::GetPos();
			newPos.z += CObjectX::GetModelMax().z * 6 - 20.0f;
			if (m_mode != 0)
			{
				Create(newPos,1);
			}
			else
			{
				Create(newPos);
			}
			this->Release();
			return;
		}
	}
	CObjectX::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CMeshGround::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CObjectX::Draw();
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

//==========================================================================================
//��������
//==========================================================================================
CMeshGround* CMeshGround::Create(D3DXVECTOR3 pos)
{
	CMeshGround* enemy = new CMeshGround;
	enemy->BindModel("data\\MODEL\\GameField.x");
	enemy->SetModelParam(pos);
	enemy->Init();
	return enemy;
}

//==========================================================================================
//��������
//==========================================================================================
CMeshGround* CMeshGround::Create(D3DXVECTOR3 pos,int i)
{
	CMeshGround* enemy = new CMeshGround;
	enemy->BindModel("data\\MODEL\\GameField_A.x");
	enemy->SetModelParam(pos);
	enemy->Init();
	enemy->m_mode = 1;
	return enemy;
}
