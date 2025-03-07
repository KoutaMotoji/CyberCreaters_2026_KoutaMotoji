//===============================================================================
//
//  ライト処理(Block.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

#define MAX_LIGHT	(3)
class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init();
	void Uninit();
	void Update();

private:
	D3DLIGHT9 m_aLight[MAX_LIGHT];
};

#endif 
