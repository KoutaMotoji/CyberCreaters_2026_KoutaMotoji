//===============================================================================
//
//  C++égÇ¡ÇΩ3D(scene.h)
//								êßçÏÅFå≥ínçOëø
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