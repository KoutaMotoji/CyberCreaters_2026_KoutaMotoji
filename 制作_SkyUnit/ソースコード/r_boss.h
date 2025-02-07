//===============================================================================
//
//  C++使った2D(t_player.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _R_BOSSX_H_
#define _R_BOSSX_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"

#include "manager.h"

static constexpr int RB_MAX_MODELPARTS = 21;

static constexpr int RB_MAX_PARTS = 21;
static constexpr int RB_MAX_KEYSET = 21;
static constexpr int RB_MAX_MOTION = 7;

class CResultBoss :public CObject
{
public:
	CResultBoss(int nPriority = 0);					//コンストラクタ
	~CResultBoss()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	void AddMove(D3DXVECTOR3 move) { m_move += move; }
	static CResultBoss* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	D3DXVECTOR3 GetPos() { return m_pos; };
private:
	D3DXVECTOR3 m_pos, m_rot, m_size;	//座標・回転・大きさ
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_OldPos;			//過去の位置
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス

	static const float MOVE_SPEED;			//移動スピードの設定

	CModelParts* m_apModelParts[RB_MAX_MODELPARTS];

	void ModelDataLoad();

	char* m_pModelFileName[RB_MAX_MODELPARTS];
	int m_ModelParts;	//モデルパーツ数

	//=====================			クオータニオン用		====================================
	D3DXMATRIX m_mtxRot;		//回転マトリックス(保存用)
	D3DXQUATERNION m_quat;		//クオータニオン
	D3DXVECTOR3 m_vecAxis;		//回転軸のベクトル
	float m_fValueRot;			//回転量
};

#endif