//===============================================================================
//
//  タイトルシーン用UI(t_UI.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _TITLEUI_H_
#define _TITLEUI_H_

#include "main.h"
#include "object2D.h"
#include "scene.h"

class CTitleUI :public CObject2D
{
public:
	CTitleUI(int nPriority = SET_PRIORITY - 1);		//コンストラクタ
	~CTitleUI()override;							//デストラクタ
	void Init()override;							//初期化
	void Uninit()override;							//終了
	void Update()override;							//更新
	void Draw()override;							//描画

	void Destroy() { CObject::Release(); return; }
	static CTitleUI* Create(D3DXVECTOR3 pos, int Type,D3DXVECTOR2 PolySize);
private:
};

class CTitleLogo : public CTitleUI
{
public:
	void Init()override;		//初期化
	void Update()override;		//更新
};

class CTitleButton : public CTitleUI
{
public:
	CTitleButton()  {}
	~CTitleButton()override = default;
	void Init()override;		//初期化
	void Update()override;		//更新

	static CTitleButton* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	void SetTex(int num);
private:
	static constexpr int MAX_TEX = 3;
	int m_TexIdx[MAX_TEX];
};

class CMoveButton : public CObject2D
{
public:
	CMoveButton(int nPriority = SET_PRIORITY - 1):CObject2D(nPriority),m_Gauge(0), m_mode((CScene::MODE)0){}
	~CMoveButton()override = default;
	void Init()override;		//初期化
	void Update()override;		//更新
	static CMoveButton* Create(CScene::MODE mode);

private:
	static constexpr int PUSHING_TIME = 60;
	static constexpr float PushGaugeWidth = 640.0f;
	static constexpr float PushGaugeHeight = PushGaugeWidth * 0.125f;
	CScene::MODE m_mode;
	int m_Gauge;
};

class CMoveButtonBack : public CObject2D
{
public:
	CMoveButtonBack(int nPriority = SET_PRIORITY - 2):CObject2D(nPriority) {}
	~CMoveButtonBack()override = default;
	void Init()override;		//初期化
	static CMoveButtonBack* Create(D3DXVECTOR3 pos);
private:
	static constexpr float PushGaugeWidth = 640.0f;
	static constexpr float PushGaugeHeight = PushGaugeWidth * 0.125f;
};

class CFinishCheck : public CObject2D
{
public:
	CFinishCheck(int nPriority = SET_PRIORITY - 1) :CObject2D(nPriority), m_fSize(0.0f), m_bPush(false){}
	~CFinishCheck()override = default;
	void Init()override;		//初期化
	void Update()override;		//初期化
	static CFinishCheck* Create(D3DXVECTOR3 pos);
private:
	static constexpr float PushGaugeWidth = 700.0f;
	static constexpr float PushGaugeHeight = 360.0f;
	float m_fSize;
	bool m_bPush;
};

#endif