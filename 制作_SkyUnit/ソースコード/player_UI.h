//===============================================================================
//
//		プレイヤー用UI(player_UI.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _GAUGE_LIFE_H_
#define _GAUGE_LIFE_H_

#include "main.h"
#include "object2D.h"

class CGaugeLife :public CObject2D
{
public:
	CGaugeLife(int nPriority = 7);		//コンストラクタ
	~CGaugeLife()override;				//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CGaugeLife* Create(int nMaxGauge);
private:
	void SetGauge();

	static const float OBJ_HEIGHT;
	static const float OBJ_WIDTH;

};

class CGaugeLiFrame :public CObject2D
{
public:
	CGaugeLiFrame(int nPriority = 7) :CObject2D(nPriority) {};		//コンストラクタ
	~CGaugeLiFrame()override = default;				//デストラクタ
	void Init()override;		//初期化

	static CGaugeLiFrame* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
};

class CGaugeLiBack :public CObject2D
{
public:
	CGaugeLiBack(int nPriority = 6) : CObject2D(nPriority) {};		//コンストラクタ
	~CGaugeLiBack()override = default;				//デストラクタ
	void Init()override;		//初期化

	static CGaugeLiBack* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
};

class CMainUI :public CObject2D
{
public:
	CMainUI(int nPriority = 5) : CObject2D(nPriority) {};		//コンストラクタ
	~CMainUI()override = default;				//デストラクタ
	void Init()override;		//初期化

	static CMainUI* Create();
	void ChangeUI(int playermode);
private:
	int m_TexIdx[2];
};

#endif