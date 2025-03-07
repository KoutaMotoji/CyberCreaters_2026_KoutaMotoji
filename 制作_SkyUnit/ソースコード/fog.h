//===============================================================================
//
//  フォグ処理(fog.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _FOG_H_
#define _FOG_H_

#include "main.h"

class CFog
{
public:
	CFog();
	~CFog();

	static void SetFogLinear(float Start,float End);	//範囲指定でフォグを作成
	static void SetFogDensity(float Density);			//密度指定でフォグを作成

	static void FinishFog();	//フォグを終了
private:

};

#endif 