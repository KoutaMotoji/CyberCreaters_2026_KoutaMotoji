//===============================================================================
// 
//  C++使った3D(tree_billboard.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _TREE_BILLBOARD_H_
#define _TREE_BILLBOARD_H_

#include "main.h"
#include "billboard.h"

namespace {
	float Poly_Radius = 300.0f;
}

static constexpr float REPLACEMENT_CHECKDIS = -2000;
static constexpr float REPLACEMENT_SETDIS = 10000;

class CTreeBillboard :public CBillboard
{
public:
	CTreeBillboard(int nPriority = 2) ;	//コンストラクタ
	~CTreeBillboard()override;			//デストラクタ
	void Init()override;				//初期化
	void Uninit()override;				//終了
	void Update()override;				//更新
	void Draw()override { CBillboard::Draw(); };		//描画

	static CTreeBillboard* Create(D3DXVECTOR3 pos,int TexIdx);
	void SetPosGround(float HalfRadius);
private:
	int m_snTexIdx[3];
	int m_nTexIdx;
};

#endif