//===============================================================================
//
//  C++使った2D(reticle.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _RETICLE_H_
#define _RETICLE_H_

#include "main.h"
#include "billboard.h"

static constexpr float Poly_Size = 25.0f;

class CReticle :public CBillboard
{
public:
	CReticle();				//コンストラクタ
	~CReticle()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CReticle* Create(D3DXVECTOR3 pos);
private:
};

#endif