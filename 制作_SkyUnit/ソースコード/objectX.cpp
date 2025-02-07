//===============================================================================
//
//  C++使った2D(objectX.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "objectX.h"
#include "manager.h"


//==========================================================================================
//コンストラクタ
//==========================================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pTextureObjectX[MAX_OBJ_TEX] = {};
	m_radius = { 0.0f,0.0f,0.0f };
}

//==========================================================================================
//デストラクタ
//==========================================================================================
CObjectX::~CObjectX()
{
}

//==========================================================================================
//初期化処理
//==========================================================================================
void CObjectX::Init()
{
}

//==========================================================================================
//終了処理
//==========================================================================================
void CObjectX::Uninit()
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
	for (int i = 0; i < (int)m_dwNumMat; i++)
	{
		if (m_pTextureObjectX[i] != nullptr)
		{
			m_pTextureObjectX[i]->Release();
			m_pTextureObjectX[i] = nullptr;
		}
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CObjectX::Update()
{
}

//==========================================================================================
//描画処理
//==========================================================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxSize;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL* pMat;

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);


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
		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================================
//描画処理
//==========================================================================================
void CObjectX::Draw(D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL* pMat;

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

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
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; ++nCntMat)
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
}

//==========================================================================================
//描画処理
//==========================================================================================
void CObjectX::Draw(D3DXMATRIX RotMtx)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL* pMat;

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);


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
		&RotMtx);
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

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================================================
//生成処理
//==========================================================================================
CObjectX* CObjectX::Create(D3DXVECTOR3 pos)
{
	CObjectX* object3D = new CObjectX;
	object3D->m_pos = pos;
	object3D->Init();
	return object3D;
}

//==========================================================================================
//生成時の初期設定処理
//==========================================================================================
void CObjectX::SetModelParam(D3DXVECTOR3 pos)
{
	m_pos = pos;
	m_size = { 1.0f,1.0f,1.0f };
	m_col = D3DCOLOR_RGBA(255, 255, 255, 255);
}

//==========================================================================================
//モデル生成処理
//==========================================================================================
void CObjectX::BindModel(const char* apFileName)	
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
	
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTextureObjectX[nCntMat]);
		}
	}

	pDevice->GetMaterial(&m_defMat);

	SetModelSize();
}

//==========================================================================================
//サイズを取得
//==========================================================================================
void CObjectX::SetModelSize()
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
		if (vtx.x >m_vtxMaxModel.x)
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
	m_pMesh->UnlockVertexBuffer();

	//直径を算出
	m_radius = (m_vtxMaxModel - m_vtxMinModel);
}