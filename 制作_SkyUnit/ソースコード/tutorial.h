//===============================================================================
//
//  C++使った3D(game.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "scene.h"
#include "mesh_obstacle.h"
#include "object2D.h"

class CTutorial : public CScene
{
public:
	CTutorial();
	~CTutorial()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

private:
	int m_nSetTimer;
};

class CTu_MoveButton : public CObject2D
{
public:
	CTu_MoveButton(int nPriority = SET_PRIORITY - 1) :CObject2D(nPriority), m_Gauge(0) {}
	~CTu_MoveButton()override = default;
	void Init()override;		//初期化
	void Update()override;		//更新
	static CTu_MoveButton* Create();

private:
	static constexpr int PUSHING_TIME = 60;
	static constexpr float PushGaugeWidth = 430.0f;
	static constexpr float PushGaugeHeight = PushGaugeWidth * 0.15f;
	int m_Gauge;
};

class CTu_MoveButtonBack : public CObject2D
{
public:
	CTu_MoveButtonBack(int nPriority = SET_PRIORITY - 2) :CObject2D(nPriority) {}
	~CTu_MoveButtonBack()override = default;
	void Init()override;		//初期化
	static CTu_MoveButtonBack* Create(D3DXVECTOR3 pos);
private:
	static constexpr float PushGaugeWidth = 430.0f;
	static constexpr float PushGaugeHeight = PushGaugeWidth * 0.15f;
};


#endif