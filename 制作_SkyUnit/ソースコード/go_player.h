//===============================================================================
//
//  C++使った3D(go_player.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _GO_PLAYERX_H_
#define _GO_PLAYERX_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"
#include "reticle.h"

#include "manager.h"


#define MAX_MODELPARTS	(20)

#define MAX_PARTS	(20)
#define MAX_KEYSET	(20)
#define MAX_MOTION	(7)

class CShadow;

class CGameOverPlayer :public CObject
{
public:
	CGameOverPlayer(int nPriority = 2);					//コンストラクタ
	~CGameOverPlayer()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	void AddMove(D3DXVECTOR3 move) { m_move += move; }
	static CGameOverPlayer* Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos() { return m_pos; };

	CShadow* m_pShadow;

private:
	D3DXVECTOR3 m_pos, m_rot, m_size;	//座標・回転・大きさ
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_OldPos;			//過去の位置
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス
	void FloorCollision();			//床との当たり判定

	static const float MOVE_SPEED;			//移動スピードの設定

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	void ModelDataLoad();

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//モデルパーツ数

	//=====================			クオータニオン用		====================================
	D3DXMATRIX m_mtxRot;		//回転マトリックス(保存用)
	D3DXQUATERNION m_quat;		//クオータニオン
	D3DXVECTOR3 m_vecAxis;		//回転軸のベクトル
	float m_fValueRot;			//回転量
};

#endif