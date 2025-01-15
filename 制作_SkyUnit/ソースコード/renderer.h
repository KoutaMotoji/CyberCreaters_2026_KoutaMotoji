//===============================================================================
//
//  C++使った2D(renderer.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "scene.h"

class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw(CScene* pScene);
	LPDIRECT3DDEVICE9 GetDevice();
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_D3DDevice;

	//フォグ用変数
	float m_fFogStartPos;
	float m_fFogEndPos;
};

#endif