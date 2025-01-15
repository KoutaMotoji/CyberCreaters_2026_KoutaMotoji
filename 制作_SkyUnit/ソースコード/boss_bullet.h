//===============================================================================
//
//  C++使った2D(object3D.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _BOSS_BULLET_H_
#define _BOSS_BULLET_H_

#include "main.h"
#include "bullet3D.h"

class CBossBullet :public CBullet3D
{
public:
	CBossBullet();				//コンストラクタ
	~CBossBullet()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CBossBullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float Radius, float EffectSize);
protected:
	bool MeshCollision()override;

private:

};

#endif