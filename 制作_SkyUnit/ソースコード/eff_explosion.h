//===============================================================================
//
//  C++使った2D(eff_exolosion.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _EFF_EXPLOSION_H_
#define _EFF_EXPLOSION_H_

#include "main.h"
#include "billboard.h"

class CEffExplosion :public CBillboard
{
public:
	CEffExplosion(int nPriority = 4);	//コンストラクタ
	~CEffExplosion()override;			//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CEffExplosion* Create(D3DXVECTOR3 pos);
	static CEffExplosion* Create(D3DXVECTOR3 pos,float Radius);

private:
	int m_nTime;
};

#endif