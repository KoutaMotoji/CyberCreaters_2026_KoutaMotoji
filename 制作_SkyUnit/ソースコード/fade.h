//===============================================================================
//
//  C++égÇ¡ÇΩ3D(fade.h)
//								êßçÏÅFå≥ínçOëø
// 
//===============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "scene.h"

class CFade
{
public:
	typedef enum
	{
		FADE_IN = 0,
		FADE_OUT,
		FADE_MAX
	}FADE_TYPE;
	CFade();
	~CFade();
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	bool GetUse();
	void SetFade(FADE_TYPE type,CScene::MODE mode);
private:
	bool FadeIn();
	bool FadeOut();
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade;

	FADE_TYPE m_FADE_type;
	CScene::MODE setmode;
	bool m_bFinish,m_bUse;
	int m_nAlpha;
	static const float ALPHA_DEC;
	static const int MAX_ALPHA;
};

#endif