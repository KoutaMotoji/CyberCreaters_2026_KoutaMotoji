//===============================================================================
//
//  3Dパーティクル処理(particle3D.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "particle3D.h"
#include "manager.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CParticle3D::CParticle3D(int nPriority) : CBillboard(nPriority) ,m_bSizeDown(true), m_move({0.0f,0.0f,0.0f})
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx));

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CParticle3D::~CParticle3D()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CParticle3D::Init()
{
	CObject::SetType(TYPE_BILLBOARD);
	CBillboard::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CParticle3D::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CParticle3D::Update()
{
	if (m_bSizeDown)
	{
		CBillboard::AddScale(-m_MinutSize);
	}

	if (m_nLifeTime > 0)
	{
		m_nLifeTime--;
	}
	else
	{
		CObject::Release();
		return;
	}
	CBillboard::AddPos(m_move);
	CBillboard::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CParticle3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//加算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Zアルファ
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CBillboard::Draw();

	//通常の合成に戻す設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zアルファ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//==========================================================================================
//生成処理
//==========================================================================================
CParticle3D* CParticle3D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float Radius,int LifeTime,bool bSizeDown)
{
	CParticle3D* particle = new CParticle3D;

	particle->SetPolygonParam(pos, Radius, Radius,col);
	particle->Init();
	particle->m_nLifeTime = LifeTime;	//残留時間
	float f = (1.0f / LifeTime);		//残留時間から1フレームの減少値を計算、下で設定
	particle->m_MinutSize = f;
	particle->m_bSizeDown = bSizeDown;	//サイズ減少の是非を設定
	return particle;
}

//==========================================================================================
//生成処理(移動量追加オーバーロード)
//==========================================================================================
CParticle3D* CParticle3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float Radius, int LifeTime, bool bSizeDown)
{
	CParticle3D* particle = new CParticle3D;

	particle->SetPolygonParam(pos, Radius, Radius, col);
	particle->Init();
	particle->m_nLifeTime = LifeTime;	//残留時間
	float f = (1.0f / LifeTime);		//残留時間から1フレームの減少値を計算、下で設定
	particle->m_MinutSize = f;
	particle->m_bSizeDown = bSizeDown;	//サイズ減少の是非を設定
	particle->m_move = move;			//移動量を設定
	return particle;
}