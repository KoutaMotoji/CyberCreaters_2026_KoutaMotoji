//===============================================================================
//
//  アイテムの処理(3D_Item.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _3DITEM_H_
#define _3DITEM_H_

#include "main.h"
#include "objectX.h"

static constexpr int MIN_CNT = 40;

class C3DItem :public CObjectX
{
public:
	C3DItem() :m_nLestTime(0), m_bGot(false) {};	//コンストラクタ
	~C3DItem()override = default;					//デストラクタ

	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	void GotThisItem() { m_bGot = true; }
	bool GetItemUse() { return m_bGot; }
	static C3DItem* Create(D3DXVECTOR3 pos);
private:
	bool m_bGot;
	int m_nLestTime;

	void GotAnimation();
};

#endif