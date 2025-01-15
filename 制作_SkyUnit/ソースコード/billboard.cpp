//===============================================================================
//
//  C++使った2D(billboard.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "billboard.h"
#include "manager.h"


//静的メンバ初期化
const float CBillboard::MAX_OBJ3DHEIGHT = 500.0f;
const float CBillboard::MAX_OBJ3DWIDTH = 500.0f;

const int CBillboard::MAX_VERTEX = 4;
const int CBillboard::MAX_POLYGON = 2;

//==========================================================================================
//コンストラクタ
//==========================================================================================
CBillboard::CBillboard(int nPriority) :CObject(nPriority), m_fWidth(0), m_fHeight(0)
{
	m_pVtxBuffBillboard = nullptr;
	m_pTextureBillboard = nullptr;
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CBillboard::~CBillboard()
{
}

//==========================================================================================
//初期化処理
//==========================================================================================
void CBillboard::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffBillboard,
		NULL);
	VERTEX_3D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth,+m_fHeight,+10.0f );
	pVtx[1].pos = D3DXVECTOR3(+m_fWidth,+m_fHeight,+10.0f );
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth,-m_fHeight,+10.0f );
	pVtx[3].pos = D3DXVECTOR3(+m_fWidth,-m_fHeight,+10.0f );

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((0.0f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_Slice.x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_Slice.y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_Slice.x), (1.0f / m_Slice.y));

	//頂点バッファのアンロック
	m_pVtxBuffBillboard->Unlock();

	m_Anim = {0,0};
}

//==========================================================================================
//終了処理
//==========================================================================================
void CBillboard::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuffBillboard != NULL)
	{
		m_pVtxBuffBillboard->Release();
		m_pVtxBuffBillboard = NULL;
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CBillboard::Update()
{
	if (m_Type == TYPE::TYPE_GAUGE)
	{
		UpdateGauge();
	}
	else
	{
		UpdateNormal();
	}
}

//==========================================================================================
//描画処理
//==========================================================================================
void CBillboard::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;
	D3DXMATRIX mtxView;

	//アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);


	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面を向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);//逆行列を求める
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;

	//大きさを反映(初期設定された大きさx,zを基準値1.0としたサイズスケーリング)
	D3DXMatrixScaling(&mtxSize,
		m_scale.y,
		m_scale.x,
		m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxSize);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	//テクスチャの設定
	pDevice->SetTexture(0, m_pTextureBillboard);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		MAX_POLYGON);

	//Zアルファ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//==========================================================================================
//生成処理
//==========================================================================================
CBillboard* CBillboard::Create(D3DXVECTOR3 pos)
{
	CBillboard* object3D = new CBillboard;
	object3D->m_pos = pos;
	object3D->SetPolygonParam(pos, MAX_OBJ3DHEIGHT, MAX_OBJ3DWIDTH);
	object3D->Init();
	return object3D;
}

//==========================================================================================
//生成時の初期設定処理
//==========================================================================================
void CBillboard::SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth)
{
	m_pos = pos;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_scale = { 1.0f,1.0f,1.0f };
	m_col = {1.0f,1.0f,1.0f,1.0f};
	//対角線の長さを算出
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_Type = CBillboard::TYPE::TYPE_NORMAL;
}

//==========================================================================================
//生成時の初期設定処理
//==========================================================================================
void CBillboard::SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth, D3DXCOLOR col)
{
	m_pos = pos;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_scale = { 1.0f,1.0f,1.0f };
	m_col = col;
	//対角線の長さを算出
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_Type = CBillboard::TYPE::TYPE_NORMAL;

}

//==========================================================================================
//生成時の初期設定処理 (ゲージタイプ用オーバーロード)
//==========================================================================================
void CBillboard::SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth, D3DXCOLOR col,int nMaxValue)
{
	m_pos = pos;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_scale = { 1.0f,1.0f,1.0f };
	m_col = col;
	m_nMaxValue = nMaxValue;
	m_nNowValue = nMaxValue;
	//対角線の長さを算出
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(m_fWidth, m_fHeight);
	m_Type = CBillboard::TYPE::TYPE_GAUGE;
}

//==========================================================================================
//生成時の初期設定処理
//==========================================================================================
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTextureBillboard = pTex;
	m_Slice = { 1.0f,1.0f };
}

//==========================================================================================
//生成時の初期設定処理(オーバーロード)
//==========================================================================================
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 pTex,D3DXVECTOR2 Slice)
{
	m_pTextureBillboard = pTex;
	m_Slice = Slice;
}

//==========================================================================================
//ゲージの長さ計算
//==========================================================================================
float CBillboard::CalcGaugeValue()
{
	float fval = 0;
	fval = (m_fWidth / m_nMaxValue) * m_nNowValue;
	
	return fval;
}

//==========================================================================================
//通常タイプの更新処理
//==========================================================================================
void CBillboard::UpdateNormal()
{
	VERTEX_3D* pVtx;	//頂点情報のポインタ

//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((1.0f / m_Slice.x) * m_Anim.x), 0.0f + ((1.0f / m_Slice.y) * m_Anim.y));
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_Slice.x) + ((1.0f / m_Slice.x) * m_Anim.x), 0.0f + ((1.0f / m_Slice.y) * m_Anim.y));
	pVtx[2].tex = D3DXVECTOR2(0.0f + ((1.0f / m_Slice.x) * m_Anim.x), (1.0f / m_Slice.y) + ((1.0f / m_Slice.y) * m_Anim.y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_Slice.x) + ((1.0f / m_Slice.x) * m_Anim.x), (1.0f / m_Slice.y) + ((1.0f / m_Slice.y) * m_Anim.y));
	//頂点バッファのアンロック
	m_pVtxBuffBillboard->Unlock();
}

//==========================================================================================
//ゲージタイプの更新処理
//==========================================================================================
void CBillboard::UpdateGauge()
{
	float fWidth;
	fWidth = CalcGaugeValue();
	float fGauge;
	fGauge = (1.0f / m_nMaxValue) * m_nNowValue;

	VERTEX_3D* pVtx;	//頂点情報のポインタ
	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);
	
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fWidth, +m_fHeight, +10.0f);
	pVtx[1].pos = D3DXVECTOR3(+fWidth, +m_fHeight, +10.0f);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, -m_fHeight, +10.0f);
	pVtx[3].pos = D3DXVECTOR3(+fWidth, -m_fHeight, +10.0f);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(fGauge, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f - fGauge, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fGauge, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f - fGauge, 1.0f);

	//頂点バッファのアンロック
	m_pVtxBuffBillboard->Unlock();
}