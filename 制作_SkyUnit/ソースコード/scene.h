//===============================================================================
//
//  シーン基底クライアント(scene.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"

class CScene
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_GAMEOVER,
		MODE_MAPEDIT,
		MOE_MAX
	}MODE;
	CScene();
	virtual ~CScene();
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	CScene::MODE GetSceneMode();
	static CScene* Create(CScene::MODE mode);
private:
	MODE m_mode;
};

#endif