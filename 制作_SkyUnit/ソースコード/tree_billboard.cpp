//===============================================================================
//
//  背景用木ビルボード処理(tree_billboard.h)
//								制作：元地弘汰
// 
//===============================================================================
#include "tree_billboard.h"
#include "mesh_ground.h"
#include "collision.h"
#include "player_observer.h"
#include "manager.h"


//==========================================================================================
//コンストラクタ
//==========================================================================================
CTreeBillboard::CTreeBillboard(int nPriority):CBillboard(nPriority)
{
	m_snTexIdx[0] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Tree000.png");
	m_snTexIdx[1] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Tree001.png");
	m_snTexIdx[2] = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Tree002.png");
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CTreeBillboard::~CTreeBillboard()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CTreeBillboard::Init()
{
	CObject::SetType(TYPE_BILLBOARD);
	CBillboard::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CTreeBillboard::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CTreeBillboard::Update()
{
	//一定間隔プレイヤーから後ろに離れた場合、X座標のみを参照して一定数後方に再設置する処理
	if (CBillboard::GetPos().z - CPlayerObserver::GetInstance()->GetPlayerPos().z <= REPLACEMENT_CHECKDIS)
	{
		D3DXVECTOR3 newPos = CBillboard::GetPos();
		newPos.z += REPLACEMENT_SETDIS;
		Create(newPos, m_nTexIdx);
		//現在のオブジェクトを破棄
		CObject::Release();
		return;
	}
	CBillboard::Update();
}

void CTreeBillboard::Draw() 
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	CBillboard::Draw(); 

	//通常の合成に戻す設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


//==========================================================================================
//生成処理
//==========================================================================================
CTreeBillboard* CTreeBillboard::Create(D3DXVECTOR3 pos, int TexIdx)
{
	CTreeBillboard* tree = new CTreeBillboard();

	tree->SetPolygonParam(pos, Poly_Radius, Poly_Radius, {1.0f,1.0f,1.0f,1.0f});
	tree->BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(tree->m_snTexIdx[TexIdx]));
	tree->m_nTexIdx = TexIdx;
	tree->Init();
	tree->AddPos({ 0.0f,-1000.0f,0.0f });
	tree->SetPosGround(Poly_Radius);
	return tree;
}

//==========================================================================================
//地形メッシュに沿って位置を合わせる処理
//==========================================================================================
void CTreeBillboard::SetPosGround(float HalfRadius)
{
	CCollision* pCollision = new CCollision();
	D3DXVECTOR3 dir = { 0.0f,1.0f,0.0f };
	float Distance = 0;

	LPD3DXMESH pMesh = nullptr;
	for (int j = 0; j < SET_PRIORITY; j++) {
		for (int i = 0; i < MAX_OBJECT; i++) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_MESHOBJECT) {
					CMeshGround* pTest = dynamic_cast<CMeshGround*>(pObj);
					if (pTest != nullptr) {
						pMesh = pTest->GetMesh();
						D3DXVECTOR3 pos = CBillboard::GetPos();

						D3DXVECTOR3 objpos = pos - pTest->GetPos();
						float ChedkDis = 2000.0f;
						// ----- 接地時処理 -----
						if (pCollision->MeshToIntersectCollision(&Distance,pMesh, objpos, dir, ChedkDis))
						{
							delete pCollision;
							pCollision = nullptr;

							CBillboard::AddPos({ 0.0f,Distance + HalfRadius,0.0f });
							return;
						}
					}
				}
			}
		}
	}
	if (pCollision != nullptr)
	{
		delete pCollision;
		pCollision = nullptr;
	}
}