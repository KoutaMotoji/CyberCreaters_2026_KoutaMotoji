//===============================================================================
//
//  C++使った2D(object3D.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _BOSS_RETICLE_H_
#define _BOSS_RETICLE_H_

#include "main.h"
#include "billboard.h"

class CBossReticle :public CBillboard
{
public:
	CBossReticle(int nPriority = 4);				//コンストラクタ
	~CBossReticle()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CBossReticle* Create(D3DXVECTOR3 pos,float Radius,int nLife,float Rotate);
	bool GetLifeState() { return m_nLife <= 0; }
private:
	int m_nLife;
	float m_RotateSpeed;
	float Poly_MinutSize;
};

#endif