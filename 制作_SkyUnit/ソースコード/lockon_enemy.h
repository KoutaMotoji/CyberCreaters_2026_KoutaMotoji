//===============================================================================
//
//		 敵のロックオン処理(lockon_enemy.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _LOCKON_ENEMY_H_
#define _LOCKON_ENEMY_H_

#include "main.h"
#include "billboard.h"


static const float POLY_RADIUS = 120.0f;

class CLockonEnemy :public CBillboard
{
public:
	CLockonEnemy();				//コンストラクタ
	~CLockonEnemy()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CLockonEnemy* Create(D3DXVECTOR3 pos);

	void SetThisPos(D3DXVECTOR3 pos) { CBillboard::SetPos(pos); }
protected:

private:

};

#endif