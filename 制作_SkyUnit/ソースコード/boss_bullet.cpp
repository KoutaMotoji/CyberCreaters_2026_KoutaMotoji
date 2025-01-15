//===============================================================================
//
//  C++�g����2D(watermelon.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "boss_bullet.h"
#include "particle3D.h"
#include "mesh_Boss_Terra.h"
#include "mesh_ground.h"
#include "mesh_obstacle.h"
#include "eff_explosion.h"
#include "collision.h"

#include "manager.h"


//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CBossBullet::CBossBullet()
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx));

}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CBossBullet::~CBossBullet()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CBossBullet::Init()
{
	CObject::SetType(TYPE_3D_BOSSBILLET);
	CBillboard::Init();
}

//==========================================================================================
//�I������
//==========================================================================================
void CBossBullet::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CBossBullet::Update()
{
	CBullet3D::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CBossBullet::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�A���t�@�e�X�g�ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	CBillboard::Draw();

	//�ʏ�̍����ɖ߂��ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================================
//��������
//==========================================================================================
CBossBullet* CBossBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float Radius, float EffectSize)
{
	CBossBullet* bullet = new CBossBullet;

	bullet->SetPolygonParam(pos, Radius, Radius, col);
	bullet->Init();
	bullet->m_move = move * 70;
	bullet->m_col = col;
	bullet->m_nLife = nLife;
	bullet->Poly_Size = Radius;
	bullet->m_EffectSize = EffectSize;
	return bullet;
}

//==========================================================================================
//���œ����蔻�菈��
//==========================================================================================
bool CBossBullet::MeshCollision()
{
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();
	D3DXVECTOR3 dir = { 0.0f,0.0f,0.0f };

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
						D3DXVec3Normalize(&dir, &m_move);

						D3DXVECTOR3 objpos = pos - pTest->GetPos();
						float ChedkDis = 20.0f;
						// ----- �ڒn������ -----
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

						float ChedkDis = 20.0f;

						if (pCollision->MeshToIntersectCollision(pTest, pos, dir, ChedkDis))
						{
							return true;
						}
					}
				}
			}
		}
	}


	return false;
}