//===============================================================================
//
//  地形メッシュの処理(mesh_ground.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "mesh_ground.h"
#include "player_observer.h"

#include "manager.h"
#include "game.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CMeshGround::CMeshGround(int nPriority) :CObjectX(nPriority), m_mode(0)
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CMeshGround::~CMeshGround()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CMeshGround::Init()
{
	CObject::SetType(TYPE_3D_MESHOBJECT);
	CObjectX::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CMeshGround::Uninit()
{
	CObjectX::Uninit();
}

//==========================================================================================
//更新処理
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
//描画処理
//==========================================================================================
void CMeshGround::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	CObjectX::Draw();
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

//==========================================================================================
//生成処理
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
//生成処理
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
