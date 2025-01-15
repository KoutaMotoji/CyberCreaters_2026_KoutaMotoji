//===============================================================================
//
//  C++使った2D(bg.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "particle.h"
#include "manager.h"

//静的メンバ初期化
const float CParticle::PARTICLE_HEIGHT = 50.0f;
const float CParticle::PARTICLE_WIDTH = 50.0f;
LPDIRECT3DTEXTURE9 CParticle::m_pTextureTemp = nullptr;

//==========================================================================================
//コンストラクタ
//==========================================================================================
CParticle::CParticle()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CParticle::~CParticle()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CParticle::Init()
{
	CObject::SetType(TYPE_BG);
	CObject2D::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CParticle::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CParticle::Update()
{
	CObject2D::SetZoom(-0.5f);
	if (CObject2D::GetSize() <= 10)
	{
		CObject::Release();
		return;
	}

	CObject2D::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//加算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//通常の合成に戻す設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================================
//生成処理
//==========================================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos)
{
	CParticle* particle = new CParticle;

	particle->BindTexture(m_pTextureTemp, 1, 1);
	particle->SetPolygonParam(pos, PARTICLE_HEIGHT, PARTICLE_WIDTH, {0.0f,0.5f,0.2f,1.0f});
	particle->Init();
	return particle;
}

//==========================================================================================
//テクスチャ生成
//==========================================================================================
void CParticle::Load()
{
	if (m_pTextureTemp == nullptr)
	{
		LPDIRECT3DDEVICE9 pDevice;
		//デバイスの取得
		pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\shadow000.jpg",
			&m_pTextureTemp);
	}
}

//==========================================================================================
//テクスチャ破棄
//==========================================================================================
void CParticle::Unload()
{
	//テクスチャの破棄
	if (m_pTextureTemp != nullptr)
	{
		m_pTextureTemp->Release();
		m_pTextureTemp = nullptr;
	}
}