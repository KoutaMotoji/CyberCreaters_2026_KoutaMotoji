//===============================================================================
//
//  ゲームオーバーシーン用UI(go_UI.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _GAMEOVERUI_H_
#define _GAMEOVERUI_H_

#include "main.h"
#include "object2D.h"

class CGameOverUI : CObject2D
{
public:
	CGameOverUI(int nPriority = 5);			//コンストラクタ
	~CGameOverUI()override;		//デストラクタ
	void Init()override;					//初期化
	void Uninit()override;					//終了
	void Update()override;					//更新
	void Draw()override;					//描画

	static CGameOverUI* Create(D3DXVECTOR3 pos,bool bSwitch);
	void ThisDelete();
private:
	int m_TexIdx[2];
};


#endif