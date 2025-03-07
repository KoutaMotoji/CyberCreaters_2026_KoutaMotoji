//===============================================================================
//
//  ゲームオーバーシーン(gameover.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "scene.h"
#include "object2D.h"
#include "score.h"
#include "go_UI.h"

class CGameover : public CScene
{
public:
	CGameover();
	~CGameover()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:
	CScore* m_pScore;
	CGameOverUI* m_pGOUI;
	bool m_bSelect;
};

class CGameoverBG :public CObject2D
{
public:
	CGameoverBG() {};		//コンストラクタ
	~CGameoverBG()override = default;				//デストラクタ
	void Init()override;		//初期化

	static CGameoverBG* Create();
private:
};

#endif