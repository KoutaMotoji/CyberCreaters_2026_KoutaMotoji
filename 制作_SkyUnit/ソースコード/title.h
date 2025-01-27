//===============================================================================
//
//  C++使った3D(title.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "scene.h"
#include "t_player.h"


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
	void UpdateAnim();
	int m_AnimTimer;
	bool m_bNowAnim;
	CTitlePlayer* m_tPl;
};

#endif