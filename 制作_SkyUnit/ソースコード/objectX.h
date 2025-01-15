//===============================================================================
//
//  C++使った2D(object3D.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "main.h"
#include "object.h"

#define MAX_OBJ_TEX	(10)

class CObjectX :public CObject
{
public:
	CObjectX(int nPriority = 1);				//コンストラクタ
	~CObjectX()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	void Draw(D3DXCOLOR col);		//描画(色変更用オーバーロード)
	void Draw(D3DXMATRIX RotMtx);	//描画(クオータニオン回転用オーバーロード)


	D3DXVECTOR3 GetPos() { return m_pos; };				//座標取得
	void AddPos(D3DXVECTOR3 pos) { m_pos += pos; };		//座標移動
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		//座標変更
	D3DXVECTOR3 GetRot() { return m_rot; };				//角度取得
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; };		//角度移動
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };		//角度移動
	void AddSize(D3DXVECTOR3 size) { m_size += size; };
	void SetSize(D3DXVECTOR3 size) { m_size = size; };
	LPD3DXMESH GetMesh() { return m_pMesh; };			//メッシュの取得
	D3DXVECTOR3* GetRadius() { return &m_radius; };		//モデルの半径取得

	D3DXVECTOR3 GetModelMin() { return m_vtxMinModel; };
	D3DXVECTOR3 GetModelMax() { return m_vtxMaxModel; };
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	static CObjectX* Create(D3DXVECTOR3 pos);
	void SetModelParam(D3DXVECTOR3 pos);					//初期設定
	void BindModel(const char* apFileName);	//モデル読み込み 
	void SetModelSize();
private:
	LPDIRECT3DTEXTURE9 m_pTextureObjectX[MAX_OBJ_TEX];
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_radius;		//半径
	D3DXCOLOR m_col;
	
	D3DMATERIAL9 m_defMat;
	D3DXMATRIX m_mtxWorld;
	LPD3DXMESH m_pMesh;
	LPD3DXBUFFER m_pBuffMat;
	DWORD m_dwNumMat;
	D3DXVECTOR3 m_vtxMinModel, m_vtxMaxModel;
};

#endif