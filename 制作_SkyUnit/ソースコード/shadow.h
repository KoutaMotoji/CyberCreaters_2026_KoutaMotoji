//===============================================================================
//
//  丸影用処理(shadow.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "object3D.h"

static constexpr float DEF_RADIUS = 70.0f;

class CShadow :public CObject3D
{
public:
	CShadow(int nPriority = 1) :CObject3D(nPriority),m_fValueRot(0){}				//コンストラクタ
	~CShadow()override = default;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override { CObject3D::Uninit(); }		//終了
	void Update()override { CObject3D::Update(); }		//更新
	void Draw()override;		//描画

	static CShadow* Create(D3DXVECTOR3 pos);
	static CShadow* Create(D3DXVECTOR3 pos, float radius);	//サイズ指定用オーバーロード

	void SetShadowGround(D3DXVECTOR3 pos);

private:
	//=====================			クオータニオン用		====================================
	D3DXMATRIX m_mtxRot;		//回転マトリックス(保存用)
	D3DXQUATERNION m_quat;		//クオータニオン
	D3DXVECTOR3 m_vecAxis;		//回転軸のベクトル
	float m_fValueRot;			//回転量	
};

#endif