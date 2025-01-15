//===============================================================================
//
//  C++使った2D(object2D.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "object2D.h"
#include "manager.h"

const int CObject2D::MAX_VERTEX = 4;
const int CObject2D::MAX_POLYGON = 2;

//==========================================================================================
//コンストラクタ
//==========================================================================================
CObject2D::CObject2D(int nPriority):CObject(nPriority), m_fWidth(0), m_fHeight(0),m_AnimU(0),m_AnimV(0),m_fZoom(0),m_GaugeAnim(0.0f,0.0f)
{
	m_pVtxBuffObject2D = nullptr;
	m_pTextureObject2D = nullptr;
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CObject2D::~CObject2D()
{
}

//==========================================================================================
//初期化処理
//==========================================================================================
void CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffObject2D,
		NULL);
	
	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffObject2D->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	
		//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((0.0f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_TexSliceU), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / m_TexSliceV));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_TexSliceU), (1.0f / m_TexSliceV));

	//頂点バッファのアンロック
	m_pVtxBuffObject2D->Unlock();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CObject2D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuffObject2D != NULL)
	{
		m_pVtxBuffObject2D->Release();
		m_pVtxBuffObject2D = NULL;
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CObject2D::Update()
{
	if (m_type == TYPE_NORMAL)
	{
		UpdateNormal();
	}
	else if (m_type == TYPE_GAUGE)
	{
		if (m_bGaugeReverse)
		{
			UpdateGaugeRight();
		}
		else
		{
			UpdateGaugeLeft();
		}
	}
}

//==========================================================================================
//描画処理
//==========================================================================================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffObject2D, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, m_pTextureObject2D);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		MAX_POLYGON);
}


//==========================================================================================
//生成処理
//==========================================================================================
CObject2D* CObject2D::Create(D3DXVECTOR3 pos)
{
	CObject2D* object2D = new CObject2D;
	object2D->m_pos = pos;
	return object2D;
}

//==========================================================================================
//生成時の初期設定処理
//==========================================================================================
void CObject2D::SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth)
{
	m_type = TYPE_NORMAL;
	m_pos = pos;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//対角線の長さを算出
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//==========================================================================================
//生成時の初期設定処理(色変更追加)
//==========================================================================================
void CObject2D::SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth, D3DXCOLOR col)
{
	m_type = TYPE_NORMAL;
	m_pos = pos;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_col = col;
	//対角線の長さを算出
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(m_fWidth, m_fHeight);
}

//==========================================================================================
//生成時の初期設定処理(ゲージ用設定追加)
//==========================================================================================
void CObject2D::SetPolygonParam(D3DXVECTOR3 pos, float fHeight, float fWidth, int nMaxGauge,bool Reverse)
{
	m_type = TYPE_GAUGE;
	m_pos = pos;
	m_fHeight = fHeight;
	m_fWidth = fWidth;
	m_nMaxGauge = nMaxGauge;
	m_nCharge = nMaxGauge;
	m_bGaugeReverse = Reverse;
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
	float fSetWidth = CalcWidth();
	//対角線の長さを算出
	m_fLength = sqrtf(fSetWidth * fSetWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(fSetWidth, m_fHeight);
}

//==========================================================================================
//テクスチャの設定(テクスチャのポインタ　/ 縦分割数 / 横分割数)
//==========================================================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex, int AnimU, int AnimV)
{
	m_pTextureObject2D = pTex;
	m_TexSliceU = AnimU;
	m_TexSliceV = AnimV;
}

//==========================================================================================
//座標取得
//==========================================================================================
D3DXVECTOR3 CObject2D::GetPos()
{
	return m_pos;
}

//==========================================================================================
//座標移動(X += Y)
//==========================================================================================
void CObject2D::AddPos(D3DXVECTOR3 pos)
{
	m_pos += pos;
}

//==========================================================================================
//座標変更( X = Y )
//==========================================================================================
void CObject2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==========================================================================================
//角度取得
//==========================================================================================
D3DXVECTOR3 CObject2D::GetRot()
{
	return m_rot;
}

//==========================================================================================
//角度変更
//==========================================================================================
void CObject2D::AddRot(D3DXVECTOR3 rot)
{
	m_rot += rot;
}

//==========================================================================================
//大きさ(対角線の長さ)取得
//==========================================================================================
float CObject2D::GetSize()
{
	return m_fLength + m_fZoom;
}

//==========================================================================================
//大きさ(対角線の長さ)変更
//==========================================================================================
void CObject2D::SetZoom(float Zoom)
{
	m_fZoom += Zoom;
}

//==========================================================================================
//アニメーション取得(現在の縦分割、横分割)
//==========================================================================================
D3DXVECTOR2 CObject2D::GetAnim()
{
	return {(float)m_AnimU,(float)m_AnimV};
}

//==========================================================================================
//アニメーション変更
//==========================================================================================
void CObject2D::SetAnim(D3DXVECTOR2 Anim)
{
	m_AnimU = (int)Anim.x;
	m_AnimV = (int)Anim.y;
}

//==========================================================================================
//ゲージ位置を設定
//==========================================================================================
void CObject2D::SetGauge(int value)
{
	m_nCharge = value;
}

//==========================================================================================
//ゲージ位置を設定
//==========================================================================================
void CObject2D::SetGaugeAnim(D3DXVECTOR2 value)
{
	m_GaugeAnim += value;
}

//==========================================================================================
//ゲージ横幅計算
//==========================================================================================
float CObject2D::CalcWidth()
{
	float fVar;

	fVar = (m_fWidth / m_nMaxGauge) * m_nCharge;

	return fVar;
}

//==========================================================================================
//更新処理(普通の2Dオブジェクト)
//==========================================================================================
void CObject2D::UpdateNormal()
{
	VERTEX_2D* pVtx;	//頂点情報のポインタ

		//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffObject2D->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.z = 0.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((1.0f / m_TexSliceU) * m_AnimU), 0.0f + ((1.0f / m_TexSliceV) * m_AnimV));
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_TexSliceU) + ((1.0f / m_TexSliceU) * m_AnimU), 0.0f + ((1.0f / m_TexSliceV) * m_AnimV));
	pVtx[2].tex = D3DXVECTOR2(0.0f + ((1.0f / m_TexSliceU) * m_AnimU), (1.0f / m_TexSliceV) + ((1.0f / m_TexSliceV) * m_AnimV));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_TexSliceU) + ((1.0f / m_TexSliceU) * m_AnimU), (1.0f / m_TexSliceV) + ((1.0f / m_TexSliceV) * m_AnimV));
	//頂点バッファのアンロック
	m_pVtxBuffObject2D->Unlock();
}

//==========================================================================================
//更新処理(ゲージタイプのオブジェクト左側に縮小タイプ)
//==========================================================================================
void CObject2D::UpdateGaugeLeft()
{
	float fWidth = CalcWidth();
	//対角線の長さを算出
	m_fLength = sqrtf(fWidth * fWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(fWidth, m_fHeight);

	//テクスチャの端を算出
	float fGaugeTex = ((1.0f / m_nMaxGauge) * m_nCharge);
	float SetPos = m_pos.x + (fWidth * 0.5f) - m_fWidth * 0.5f;

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffObject2D->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	
	//頂点座標の設定
	pVtx[0].pos.x = SetPos + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = SetPos + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = SetPos + sinf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = SetPos + sinf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.z = 0.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + m_GaugeAnim.x, 0.0f + m_GaugeAnim.y);
	pVtx[1].tex = D3DXVECTOR2(fGaugeTex + m_GaugeAnim.x, 0.0f + m_GaugeAnim.y);
	pVtx[2].tex = D3DXVECTOR2(0.0f + m_GaugeAnim.x, 1.0f + m_GaugeAnim.y);
	pVtx[3].tex = D3DXVECTOR2(fGaugeTex + m_GaugeAnim.x, 1.0f + m_GaugeAnim.y);
	//頂点バッファのアンロック
	m_pVtxBuffObject2D->Unlock();
}

//==========================================================================================
//更新処理(ゲージタイプのオブジェクト右側に縮小タイプ)
//==========================================================================================
void CObject2D::UpdateGaugeRight()
{
	float fWidth = CalcWidth();
	//対角線の長さを算出
	m_fLength = sqrtf(fWidth * fWidth + m_fHeight * m_fHeight) / 2.0f;
	//対角線の角度を算出
	m_fAngle = atan2f(fWidth, m_fHeight);

	//テクスチャの端を算出
	float fGaugeTex = ((1.0f / m_nMaxGauge) * m_nCharge);
	float SetPos = m_pos.x - (fWidth * 0.5f) + m_fWidth * 0.5f;

	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	m_pVtxBuffObject2D->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定

	//頂点座標の設定
	pVtx[0].pos.x = SetPos + sinf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z - (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = SetPos + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + (D3DX_PI - m_fAngle)) * (m_fLength + m_fZoom);
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = SetPos + sinf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z - m_fAngle) * (m_fLength + m_fZoom);
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = SetPos + sinf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + m_fAngle) * (m_fLength + m_fZoom);
	pVtx[3].pos.z = 0.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(( 1.0f - fGaugeTex) + m_GaugeAnim.x, 0.0f + m_GaugeAnim.y);
	pVtx[1].tex = D3DXVECTOR2(1.0f + m_GaugeAnim.x, 0.0f + m_GaugeAnim.y);
	pVtx[2].tex = D3DXVECTOR2((1.0f - fGaugeTex) + m_GaugeAnim.x, 1.0f + m_GaugeAnim.y);
	pVtx[3].tex = D3DXVECTOR2(1.0f + m_GaugeAnim.x, 1.0f + m_GaugeAnim.y);
	//頂点バッファのアンロック
	m_pVtxBuffObject2D->Unlock();
}