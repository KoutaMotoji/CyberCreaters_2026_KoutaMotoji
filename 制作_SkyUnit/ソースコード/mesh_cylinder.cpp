//===============================================================================
//
//  C++使った2D(field.cpp)
//								制作：元地弘汰
//
//===============================================================================
#include "mesh_cylinder.h"
#include "player_observer.h"

#include "manager.h"

//静的メンバ初期化
const float CMeshCylinder::MAX_WIDTH = 12000.0f;
const float CMeshCylinder::MAX_HEIGHT = 10000.0f;


const int CMeshCylinder::MAX_CORNER = 8;

const int CMeshCylinder::MAX_VTX = MAX_CORNER * 2;
const int CMeshCylinder::MAX_INDEX = MAX_VTX  + 2;

//==========================================================================================
//コンストラクタ
//==========================================================================================
CMeshCylinder::CMeshCylinder(int nPriority): CObject(nPriority),m_pos({0.0f,0.0f,0.0f}), m_rot({0.0f,0.0f,0.0f})
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CMeshCylinder::~CMeshCylinder()
{

}

//==========================================================================================
//初期化処理
//==========================================================================================
void CMeshCylinder::Init()
{
	//各ポインタの初期化
	m_apTexMeshCylinder = nullptr;
	m_pMesh = nullptr;
	//m_pVtxBuffMeshCylinder = nullptr;
	//m_pIdxBuffMeshCylinder = nullptr;

	CObject::SetType(TYPE_3D_MADEMESH);

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky_bg001.png",
		&m_apTexMeshCylinder);

	D3DXCreateMeshFVF(
		(DWORD)MAX_VTX,
		(DWORD)MAX_VTX,
		D3DXMESH_WRITEONLY | D3DXMESH_MANAGED,
		FVF_VERTEX_3D,
		pDevice,
		&m_pMesh);
	
	VERTEX_3D* pVtx;	//頂点情報のポインタ
	
	//メッシュの頂点バッファのロック
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);

	for (int i = 0; i < 2; ++i)	{
		for (int j = 0; j < MAX_CORNER; ++j)	{
			float radian = (((float)j) / (float)MAX_CORNER);
			int nNum = ((i * MAX_CORNER) + j);
			pVtx[nNum].pos.x = m_pos.x + (cosf(radian  * 2 * D3DX_PI) * MAX_WIDTH);
			pVtx[nNum].pos.z = m_pos.z + (sinf(radian  * 2 * D3DX_PI) * MAX_WIDTH);
			pVtx[nNum].pos.y = m_pos.y + i * -MAX_HEIGHT;
		}
	}
	for (int i = 0; i < MAX_VTX; ++i)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < MAX_CORNER; ++j) {
			int nNum = ((i * MAX_CORNER) + j);

			pVtx[nNum].tex = {
				(float)(( 1.0f / (MAX_CORNER)) * j  *2),
				(float)(1.0f * i)

			};
		}
	}
	//メッシュの頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	int nLoop = 0;

	WORD* pIdx;	//インデックス情報のポインタ
	//メッシュのインデックスバッファのロック
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pIdx);

	for (int X = 0; X < MAX_CORNER - 1; ++X)
	{
		pIdx[nLoop] = (X + MAX_CORNER);
		++nLoop;

		pIdx[nLoop] = X;
		++nLoop;

		pIdx[nLoop] = (X + 1 + MAX_CORNER);
		++nLoop;
		

		pIdx[nLoop] = X;
		++nLoop;

		pIdx[nLoop] = (X + 1 + MAX_CORNER);
		++nLoop;

		pIdx[nLoop] = X + 1;
		++nLoop;
	}

	pIdx[nLoop] = (MAX_VTX - 1);
	++nLoop;

	pIdx[nLoop] = (MAX_CORNER - 1);
	++nLoop;

	pIdx[nLoop] = (MAX_CORNER);
	++nLoop;

	pIdx[nLoop] = (MAX_CORNER - 1);
	++nLoop;

	pIdx[nLoop] = (MAX_CORNER);
	++nLoop;

	pIdx[nLoop] = 0;

	//メッシュのインデックスバッファのアンロック
	m_pMesh->UnlockIndexBuffer();
}

//==========================================================================================
//終了処理
//==========================================================================================
void CMeshCylinder::Uninit()
{
	if (m_apTexMeshCylinder != nullptr)
	{
		m_apTexMeshCylinder->Release();
		m_apTexMeshCylinder = nullptr;
	}
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}
}

//==========================================================================================
//更新処理
//==========================================================================================
void CMeshCylinder::Update()
{
	VERTEX_3D* pVtx;	//頂点情報のポインタ

	//メッシュの頂点バッファのロック
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pVtx);

	for (int i = 0; i < MAX_VTX; ++i)
	{
		pVtx[i].tex.x += 0.00015f;
	}

	//メッシュの頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	m_pos.z = CPlayerObserver::GetInstance()->GetPlayerPos().z;
}

//==========================================================================================
//描画処理
//==========================================================================================
void CMeshCylinder::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtx);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtx,
		&m_mtx,
		&mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtx,
		&m_mtx,
		&mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtx);

	pDevice->SetTexture(0, m_apTexMeshCylinder);


	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//カリングを両面に

	m_pMesh->DrawSubset(0);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//カリング戻し

}


//==========================================================================================
//生成処理
//==========================================================================================
CMeshCylinder* CMeshCylinder::Create(D3DXVECTOR3 pos)
{
	CMeshCylinder* field = new CMeshCylinder;
	field->Init();
	field->m_pos = pos;
	field->m_pos.y += MAX_HEIGHT * 0.6;
	return field;
}
