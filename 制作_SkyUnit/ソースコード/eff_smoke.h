//===============================================================================
//
//  C++使った2D(eff_exolosion.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _EFF_SMOKE_H_
#define _EFF_SMOKE_H_

#include "main.h"
#include "billboard.h"

class CEffSmoke :public CBillboard
{
public:
	CEffSmoke(int nPriority = 1);	//コンストラクタ
	~CEffSmoke()override;			//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CEffSmoke* Create(D3DXVECTOR3 pos, float Radius,int nLife);

private:
	D3DXVECTOR3 SetRandValue();
	D3DXVECTOR3 m_moveValue;
	float m_fRotValue;
	
	int m_nLife;
	int m_nTime;
};

#endif