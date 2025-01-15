//===============================================================================
//
//  C++�g����2D(TitleBg.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _TITLEUI_H_
#define _TITLEUI_H_

#include "main.h"
#include "object2D.h"

static constexpr float SWAP_SPEED = 0.02f;
static constexpr int PUSHING_TIME = 60;
static constexpr float PushGaugeWidth = 640.0f;
static constexpr float PushGaugeHeight = PushGaugeWidth * 0.125f;

class CTitleUI :public CObject2D
{
public:
	CTitleUI(int nPriority = SET_PRIORITY - 1);		//�R���X�g���N�^
	~CTitleUI()override;							//�f�X�g���N�^
	void Init()override;							//������
	void Uninit()override;							//�I��
	void Update()override;							//�X�V
	void Draw()override;							//�`��

	static CTitleUI* Create(D3DXVECTOR3 pos, int Type,D3DXVECTOR2 PolySize);
private:
};

class CTitleLogo : public CTitleUI
{
public:
	void Init()override;		//������
	void Update()override;		//�X�V
};

class CTitleButton : public CTitleUI
{
public:
	CTitleButton() : m_LocalCol({ 1.0f,1.0f,1.0f,1.0f }), m_bColSwitch(false) {}
	~CTitleButton()override = default;
	void Init()override;		//������
	void Update()override;		//�X�V
private:
	void AlphableUI();
	bool m_bColSwitch;
	D3DXCOLOR m_LocalCol;
};

class CMoveButton : public CObject2D
{
public:
	CMoveButton(int nPriority = SET_PRIORITY - 1):CObject2D(nPriority),m_Gauge(0){}
	~CMoveButton()override = default;
	void Init()override;		//������
	void Update()override;		//�X�V
	static CMoveButton* Create();

private:

	int m_Gauge;
};

class CMoveButtonBack : public CObject2D
{
public:
	CMoveButtonBack(int nPriority = SET_PRIORITY - 2):CObject2D(nPriority) {}
	~CMoveButtonBack()override = default;
	void Init()override;		//������
	static CMoveButtonBack* Create(D3DXVECTOR3 pos);

};
#endif