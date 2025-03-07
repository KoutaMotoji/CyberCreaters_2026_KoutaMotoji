//===============================================================================
//
//  モーション用モデルのパーツ処理(modelparts.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "modelparts.h"
#include "manager.h"


//==========================================================================================
//コンストラクタ
//==========================================================================================
CModelParts::CModelParts()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CModelParts::~CModelParts()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
HRESULT CModelParts::Init()
{
	m_pParent = nullptr;

	for (int i = 0; i < MAX_MODEL_TEX; ++i)
	{
		m_pTextureObjectX[i] = nullptr;
	}
	return S_OK;
}

//==========================================================================================
//終了処理
//==========================================================================================
void CModelParts::Uninit()
{
	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
	for (int i = 0; i < (int)m_dwNumMat; ++i)
	{
		if (m_pTextureObjectX[i] != nullptr)
		{
			m_pTextureObjectX[i]->Release();
			m_pTextureObjectX[i] = nullptr;
		}
	}
}


//==========================================================================================
//描画処理
//==========================================================================================
void CModelParts::Draw()
{



	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	
	//アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL* pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxSize,
		m_size.y,
		m_size.x,
		m_size.z);
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
	//親の行列を取得する
	D3DXMATRIX mtxParent;
	if (m_pParent != nullptr)
	{
		//親のワールド変換行列を取得
		mtxParent = m_pParent->GetWorldMatrix();
	}
	else
	{
		//最新のワールドマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	//ワールド行列を親の行列に掛ける
	D3DXMatrixMultiply(&m_mtxWorld,
						&m_mtxWorld,
						&mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; ++nCntMat)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTextureObjectX[nCntMat]);

		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//通常の合成に戻す設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================================
//生成処理
//==========================================================================================
CModelParts* CModelParts::Create(D3DXVECTOR3 pos, const char* apFileName)
{
	CModelParts* ModelParts = new CModelParts;
	ModelParts->m_pos = pos;
	ModelParts->m_rot = { 0.0f,0.0f,0.0f };
	ModelParts->m_size = { 1.0f,1.0f,1.0f };
	ModelParts->BindModel(apFileName);
	ModelParts->Init();
	ModelParts->m_nIdx = -1;
	return ModelParts;
}

//==========================================================================================
//モデルの親設定
//==========================================================================================
void CModelParts::SetParent(CModelParts* pParent)
{
	if (!(pParent->m_nIdx == -1))
	{
		m_pParent = pParent;
	}
}

//==========================================================================================
//モデル生成処理
//==========================================================================================
void CModelParts::BindModel(const char* apFileName)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(apFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; ++nCntMat)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTextureObjectX[nCntMat]);
		}
	}
	SetModelSize();
}


//==========================================================================================
//サイズを取得
//==========================================================================================
void CModelParts::SetModelSize()
{
	int nNumVtx;
	DWORD sizeFVF;
	BYTE* pVtxBuff;

	//頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int i = 0; i < nNumVtx; i++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
		if (vtx.x < m_vtxMinModel.x)
		{
			m_vtxMinModel.x = vtx.x;
		}
		if (vtx.y < m_vtxMinModel.y)
		{
			m_vtxMinModel.y = vtx.y;
		}
		if (vtx.z < m_vtxMinModel.z)
		{
			m_vtxMinModel.z = vtx.z;
		}
		if (vtx.x > m_vtxMaxModel.x)
		{
			m_vtxMaxModel.x = vtx.x;
		}
		if (vtx.y > m_vtxMaxModel.y)
		{
			m_vtxMaxModel.y = vtx.y;
		}
		if (vtx.z > m_vtxMaxModel.z)
		{
			m_vtxMaxModel.z = vtx.z;
		}
		//頂点フォーマットのサイズ分ポイン多を進める
		pVtxBuff += sizeFVF;
	}
	//直径を算出
	m_radius = (m_vtxMaxModel - m_vtxMinModel);
}

//==========================================================================================
//描画処理(オーバーロード)
//==========================================================================================
void CModelParts::Draw(float alpha)
{



	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL* pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxSize,
		m_size.y,
		m_size.x,
		m_size.z);
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
	//親の行列を取得する
	D3DXMATRIX mtxParent;
	if (m_pParent != nullptr)
	{
		//親のワールド変換行列を取得
		mtxParent = m_pParent->GetWorldMatrix();
	}
	else
	{
		//最新のワールドマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	//ワールド行列を親の行列に掛ける
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTextureObjectX[nCntMat]);

		pMat[nCntMat].MatD3D.Diffuse.a = alpha;

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//通常の合成に戻す設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================================
//描画処理(オーバーロード)
//==========================================================================================
void CModelParts::Draw(D3DXCOLOR col)
{



	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//アルファテスト設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL* pMat;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//大きさを反映
	D3DXMatrixScaling(&mtxSize,
		m_size.y,
		m_size.x,
		m_size.z);
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
	//親の行列を取得する
	D3DXMATRIX mtxParent;
	if (m_pParent != nullptr)
	{
		//親のワールド変換行列を取得
		mtxParent = m_pParent->GetWorldMatrix();
	}
	else
	{
		//最新のワールドマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	//ワールド行列を親の行列に掛ける
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		D3DMATERIAL9 pMatCopy = (pMat[nCntMat].MatD3D);
		pMatCopy.Diffuse = col;
		//マテリアルの設定
		pDevice->SetMaterial(&pMatCopy);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTextureObjectX[nCntMat]);

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//通常の合成に戻す設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}