//===============================================================================
//
//  タイトルシーン(title.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "t_player.h"
#include "objectX.h"

class CStarter;
class CTitleButton;
class CTitleUI;

class CTitle : public CScene
{
public:
	CTitle();
	~CTitle()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:
	void UpdateAnim(int m);
	int m_AnimTimer;
	bool m_bNowAnim;
	CTitlePlayer* m_tPl;
	int m_nSelect;
	CStarter* m_tSt;
	CTitleButton* m_tBut;
	CTitleUI* m_tUI;

	enum
	{
		SELECT_GAME = 0,
		SELECT_TUTORIAL,
		SELECT_FINISH
	};
};

class CT_Obj : public CObjectX
{
public:
	CT_Obj() {};
	~CT_Obj()override = default;

	static CT_Obj* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
};

#endif