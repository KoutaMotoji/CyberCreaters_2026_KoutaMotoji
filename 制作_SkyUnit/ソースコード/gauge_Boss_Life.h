//===============================================================================
//
//  ボスの体力ゲージ処理(gauge_Boss_Life.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _GAUGE_BOSS_LIFE_H_
#define _GAUGE_BOSS_LIFE_H_

#include "billboard.h"

class CGaugeBoss : public CBillboard
{
public:
	CGaugeBoss(int nPriority = 4);
	~CGaugeBoss();

	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	static CGaugeBoss* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size,int MaxValue);
private:

};


#endif