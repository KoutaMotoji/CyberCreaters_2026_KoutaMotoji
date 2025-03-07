//===============================================================================
//
//  プレイヤー用レーザー弾処理(bullet3D.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "bullet3D.h"
#include "particle3D.h"
#include "enemy_base.h"
#include "mesh_Boss_Terra.h"
#include "mesh_ground.h"
#include "mesh_obstacle.h"
#include "mesh_cylinder.h"
#include "eff_explosion.h"
#include "collision.h"

#include "manager.h"


//==========================================================================================
//コンストラクタ
//==========================================================================================
CBullet3D::CBullet3D(int nPriority) : CBillboard(nPriority),m_nLife(1), Poly_Size(0.0f),m_EffectSize(0.0f)
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx));

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CBullet3D::~CBullet3D()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CBullet3D::Init()
{
	CObject::SetType(TYPE_BILLBOARD);
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_SHOT);
	CBillboard::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CBullet3D::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CBullet3D::Update()
{
	if (m_nLife > 0)
	{
		CBillboard::AddPos(m_move);
		CParticle3D::Create(CBillboard::GetPos(), m_col, Poly_Size, m_EffectSize * 0.5f,false);
		CParticle3D::Create(CBillboard::GetPos(), {1.0f,1.0f,1.0f,0.8f}, Poly_Size * 0.5f, m_EffectSize * 0.5f, false);
		if (MeshCollision())
		{
			CEffExplosion::Create(CBillboard::GetPos());
			CObject::Release();
			return;
		}
		m_nLife--;
	}
	else
	{
		CObject::Release();
		return;
	}


	CBillboard::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CBullet3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//加算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

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
CBullet3D* CBullet3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,int nLife,float Radius,float EffectSize)
{
	CBullet3D* bullet = new CBullet3D;

	bullet->SetPolygonParam(pos, Radius, Radius,col);
	bullet->Init();
	bullet->m_move = move * 70;
	bullet->m_col = col;
	bullet->m_nLife = nLife ;
	bullet->Poly_Size = Radius;
	bullet->m_EffectSize = EffectSize;
	return bullet;
}

//==========================================================================================
//自滅当たり判定処理
//==========================================================================================
bool CBullet3D::MeshCollision()
{
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();
	if (pCollision == nullptr)
	{
		return false;
	}
	D3DXVECTOR3 dir = { 0.0f,0.0f,0.0f };

	LPD3DXMESH pMesh = nullptr;
	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_MESHOBJECT) {
					CMeshGround* pTest = dynamic_cast<CMeshGround*>(pObj);
					if (pTest != nullptr) {
						pMesh = pTest->GetMesh();
						D3DXVECTOR3 pos = CBillboard::GetPos();
						D3DXVec3Normalize(&dir, &m_move);
						float fLength = D3DXVec3Length(&m_move);
						float ChedkDis = 10.0f + fLength;
						D3DXVECTOR3 objpos = pos - pTest->GetPos();

						// ----- 接地時処理 -----
						if (pCollision->MeshToIntersectCollision(pMesh, objpos, dir, ChedkDis))
						{
							return true;
						}						
					}
				}
				else if (type == CObject::TYPE::TYPE_3D_OBSTACLE) {

					CMeshObstacle* pTest = dynamic_cast<CMeshObstacle*>(pObj);
					if (pTest != nullptr) {

						pMesh = pTest->GetMesh();
						D3DXVECTOR3 pos = CBillboard::GetPos();
						D3DXVec3Normalize(&dir, &m_move);
						float fLength = D3DXVec3Length(&m_move);
						float ChedkDis = 10.0f + fLength;

						if (pCollision->MeshToIntersectCollision(pTest, pos, dir, ChedkDis))
						{
							return true;
						}
					}
				}
				else if (type == CObject::TYPE::TYPE_3D_BOSSTERRA) {

					CBossTerra* pTest = dynamic_cast<CBossTerra*>(pObj);
					if (pTest != nullptr) {
						for (int i = 0; i < 19; ++i)
						{
							pMesh = pTest->GetBossMesh(i);
							D3DXVECTOR3 pos = CBillboard::GetPos();
							D3DXVec3Normalize(&dir, &m_move);
							BOOL  bIsHit = false;
							float fLandDistance;
							float ChedkDis = 20.0f;
							D3DXMATRIX mWorld;
							D3DXVECTOR3 vStartl;
							D3DXVECTOR3 vDirl;
							D3DXVECTOR3 vEnd;

							vEnd = pos + dir;
							D3DXMATRIX objMtx = pTest->GetBossMtx(i);

							// レイを当てる対象のマトリックスの逆行列を取得し、始点と終点の座標に対して座標変換を行い、位置・回転・大きさの補間をする
							D3DXMatrixInverse(&mWorld, NULL, &objMtx);
							D3DXVec3TransformCoord(&vStartl, &pos, &mWorld);
							D3DXVec3TransformCoord(&vEnd, &vEnd, &mWorld);

							vDirl = vEnd - vStartl;

							D3DXIntersect(pMesh, &vStartl, &vDirl, &bIsHit, nullptr, nullptr, nullptr, &fLandDistance, nullptr, nullptr);

							if (bIsHit && fLandDistance < ChedkDis)
							{
								if (!pTest->GetDamageState() && !pTest->GetDeadState())
								{
									pTest->Damaged(40);
								}
								return true;
							}
						}
					}
				}
				else if (type == CObject::TYPE::TYPE_3D_ENEMY) {

					CEnemyBase* pTest = dynamic_cast<CEnemyBase*>(pObj);
					if (pTest != nullptr) {
						D3DXVECTOR3 dirM = D3DXVECTOR3(100.0f, 100.0f, 100.0f);

						if (pCollision->SphireCollosion(CBillboard::GetPos(), pTest->GetPos(), dirM, pTest->GetModelMax()))
						{
							pTest->Damaged();	
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}