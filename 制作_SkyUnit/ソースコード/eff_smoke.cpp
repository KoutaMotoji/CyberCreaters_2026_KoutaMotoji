//===============================================================================
//
//  C++使った2D(eff_smoke.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "eff_smoke.h"
#include "manager.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CEffSmoke::CEffSmoke(int nPriority) :CBillboard(nPriority), m_nTime(0)
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\smoke.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), { 1,1 });
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CEffSmoke::~CEffSmoke()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CEffSmoke::Init()
{
	CObject::SetType(TYPE_BILLBOARD);
	m_moveValue = SetRandValue();
	CBillboard::Init();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CEffSmoke::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//更新処理
//==========================================================================================
void CEffSmoke::Update()
{
	--m_nLife;
	if (m_nLife < 0)
	{
		CObject::Release();
		return;
	}
	CBillboard::AddPos(m_moveValue);
	CBillboard::AddRot({ 0.0f,0.0f,m_fRotValue });
	CBillboard::AddScale(0.05f);

	CBillboard::Update();
}

//==========================================================================================
//描画処理
//==========================================================================================
void CEffSmoke::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	// 法線の自動正規化を有効に
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//カリングを両面に

	CBillboard::Draw();

	// 法線の自動正規化を無効に
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//カリングをもどすに

}

//==========================================================================================
//生成処理
//==========================================================================================
CEffSmoke* CEffSmoke::Create(D3DXVECTOR3 pos, float Radius,int nLife)
{
	CEffSmoke* Effect = new CEffSmoke;

	Effect->SetPolygonParam(pos, Radius, Radius);
	Effect->Init();
	Effect->m_nLife = nLife;
	Effect->m_nTime = 0;
	return Effect;
}

//==========================================================================================
//生成処理
//==========================================================================================
D3DXVECTOR3 CEffSmoke::SetRandValue()
{
	D3DXVECTOR3 p;
	CEffSmoke* Effect = new CEffSmoke;

	std::random_device rnd;			// 非決定的な乱数生成器でシード生成機を生成
	std::mt19937 mt(rnd());			//  メルセンヌツイスターの32ビット版、引数は初期シード
	std::uniform_int_distribution<> rand_x(-8, 8);	// [-3000, 3000] 範囲の一様乱数
	std::uniform_int_distribution<> rand_y(10, 30);		// [2000, 5000] 範囲の一様乱数
	std::uniform_int_distribution<> rand_z(-8, 8);	// [2000, 5000] 範囲の一様乱数
	std::uniform_int_distribution<> rotV(800, 1500);	// [2000, 5000] 範囲の一様乱数
	std::uniform_int_distribution<> addSize(80, 150);	// [2000, 5000] 範囲の一様乱数

	p = {
		(float)(rand_x(mt))* 0.15f,
		(float)(rand_y(mt))* 0.15f,
		(float)(rand_z(mt))* 0.15f
	};

	m_fRotValue = ((float)rotV(mt) * 0.0001);
	AddScale((float)(addSize(mt) * 0.008f));
	return p;
}