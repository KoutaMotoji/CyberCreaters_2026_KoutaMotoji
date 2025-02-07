//===============================================================================
//
//  C++使った2D(eff_bomb.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "eff_bomb.h"
#include "manager.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CEffBomb::CEffBomb(int nPriority) :CBillboard(nPriority), m_nTime(0)
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\bomb001.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), { 5,3 });
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CEffBomb::~CEffBomb()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CEffBomb::Init()
{
	CObject::SetType(TYPE_BILLBOARD);
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_BOMB);
	CBillboard::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CEffBomb::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CEffBomb::Update()
{
	if (m_nTime > 2)
	{
		D3DXVECTOR2 Anim = CBillboard::GetAnim();

		m_nTime = 0;

		if (Anim.x >= 4)
		{
			CBillboard::AddAnim({ -5,1 });
		}
		if (Anim.y >= 2 && Anim.x >= 4)
		{
			Release();
			return;
		}
		if (Anim.x < 5 &&
			Anim.y <= 2)
		{
			CBillboard::AddAnim({ 1,0 });
		}
	}
	else
	{
		m_nTime++;
	}
	CBillboard::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CEffBomb::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//Zアルファ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CBillboard::Draw();

	//Zアルファ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//==========================================================================================
//生成処理
//==========================================================================================
CEffBomb* CEffBomb::Create(D3DXVECTOR3 pos)
{
	CEffBomb* Effect = new CEffBomb;

	Effect->SetPolygonParam(pos, 60.0f, 60.0f);
	Effect->Init();
	Effect->m_nTime = 0;

	return Effect;
}

//==========================================================================================
//生成処理
//==========================================================================================
CEffBomb* CEffBomb::Create(D3DXVECTOR3 pos, float Radius)
{
	CEffBomb* Effect = new CEffBomb;

	Effect->SetPolygonParam(pos, Radius, Radius);
	Effect->Init();
	Effect->m_nTime = 0;

	return Effect;
}
