//===============================================================================
//
//  C++使った3Dゲーム(modelparts.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _MODELPARTS_H_
#define _MODELPARTS_H_

#include "main.h"

#define MAX_MODEL_TEX	(10)

class CModelParts 
{
public:
	CModelParts();		//コンストラクタ
	~CModelParts();		//デストラクタ
	HRESULT Init();
	void Uninit();
	void Draw();
	void Draw(float alpha);
	void Draw(D3DXCOLOR col);

	static CModelParts* Create(D3DXVECTOR3 pos,const char* apFileName);
	void SetParent(CModelParts* pParent);

	void BindModel(const char* apFileName);	//モデル読み込み 
	void SetModelSize();								//モデルサイズ最大値・最小値を取得

	D3DXVECTOR3& GetPos() { return m_pos; };			//モデル位置取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		//モデル位置変更
	D3DXVECTOR3& GetRot() { return m_rot; };			//モデル回転取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		//モデル回転変更
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; };
	D3DXVECTOR3& GetSize() { return m_size; };			//モデル拡縮取得
	void SetSize(D3DXVECTOR3 size) { m_size = size; };	//モデル拡縮変更
	void SetDefault() { m_defaultPos = m_pos; m_defaultRot = m_rot; };	//位置・回転初期値設定

	int GetIndex() { return m_nIdx; };					//インデックス番号取得
	void SetIndex(int Idx) { m_nIdx = Idx; };			//インデックス番号設定
	D3DXVECTOR3 GetDefaultPos() { return m_defaultPos; };	//位置初期値取得
	D3DXVECTOR3 GetDefaultRot() { return m_defaultRot; };	//回転初期値取得
	D3DXMATRIX& GetWorldMatrix() { return m_mtxWorld; };	//マトリックス取得

	D3DXVECTOR3 GetModelMin() { return m_vtxMinModel; }		//最大値取得
	D3DXVECTOR3 GetModelMax() { return m_vtxMaxModel; }		//最小値取得

	D3DXVECTOR3* GetRadius() { return &m_radius; };			//半径取得
	LPD3DXMESH GetMesh() { return m_pMesh; }
private:
	//Xファイル管理に必要なデータ
	LPDIRECT3DTEXTURE9 m_pTextureObjectX[MAX_MODEL_TEX];	//モデルに貼り付けるテクスチャ管理
	LPD3DXMESH m_pMesh;				//モデルのメッシュ情報
	LPD3DXBUFFER m_pBuffMat;		//モデルのバッファ
	DWORD m_dwNumMat;				//マテリアル
	D3DXVECTOR3 m_vtxMinModel, m_vtxMaxModel;	//モデルのサイズ格納用

	//オブジェクト情報
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_defaultPos;	//初期位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_defaultRot;	//初期の向き
	D3DXVECTOR3 m_size;			//大きさ
	D3DXVECTOR3 m_radius;		//半径

	int m_nIdx;
	CModelParts* m_pParent;		//親のモデルのポインタ
};

#endif