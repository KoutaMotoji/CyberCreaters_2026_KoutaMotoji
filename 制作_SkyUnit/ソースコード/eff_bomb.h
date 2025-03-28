//===============================================================================
//
//  被弾時爆発エフェクト処理(eff_bomb.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _EFF_BOMB_H_
#define _EFF_BOMB_H_

#include "main.h"
#include "billboard.h"

class CEffBomb :public CBillboard
{
public:
	CEffBomb(int nPriority = 4);				//コンストラクタ
	~CEffBomb()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CEffBomb* Create(D3DXVECTOR3 pos);
	static CEffBomb* Create(D3DXVECTOR3 pos, float Radius);

private:
	int m_nTime;
};

#endif