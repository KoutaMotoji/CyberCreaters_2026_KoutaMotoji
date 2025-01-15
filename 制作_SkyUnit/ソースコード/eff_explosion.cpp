//===============================================================================
//
//  C++�g����2D(eff_explosion.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "eff_explosion.h"
#include "manager.h"

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CEffExplosion::CEffExplosion(int nPriority) :CBillboard(nPriority) , m_nTime(0)
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\bomb000.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), { 8,2 });
}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CEffExplosion::~CEffExplosion()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CEffExplosion::Init()
{
	CObject::SetType(TYPE_BILLBOARD);
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_BOMB);
	CBillboard::Init();
}

//==========================================================================================
//�I������
//==========================================================================================
void CEffExplosion::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CEffExplosion::Update()
{
	if (m_nTime > 2)
	{
		D3DXVECTOR2 Anim = CBillboard::GetAnim();

		m_nTime = 0;

		if (Anim.x >= 7)
		{
			CBillboard::AddAnim({ -8,1 });
		}
		if (Anim.y >= 1 && Anim.x >= 7)
		{
			Release();
			return;
		}
		if(Anim.x < 8&&
			Anim.y <= 1)
		{
			CBillboard::AddAnim({ 1,0 });
		}
		Anim = Anim;
	}
	else
	{
		m_nTime++;
	}
	CBillboard::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CEffExplosion::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//Z�A���t�@
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CBillboard::Draw();

	//Z�A���t�@
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//==========================================================================================
//��������
//==========================================================================================
CEffExplosion* CEffExplosion::Create(D3DXVECTOR3 pos)
{
	CEffExplosion* Effect = new CEffExplosion;

	Effect->SetPolygonParam(pos, 60.0f, 60.0f);
	Effect->Init();
	Effect->m_nTime = 0;
	return Effect;
}

//==========================================================================================
//��������
//==========================================================================================
CEffExplosion* CEffExplosion::Create(D3DXVECTOR3 pos,float Radius)
{
	CEffExplosion* Effect = new CEffExplosion;

	Effect->SetPolygonParam(pos, Radius, Radius);
	Effect->Init();
	Effect->m_nTime = 0;
	return Effect;
}
