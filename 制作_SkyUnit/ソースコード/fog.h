//===============================================================================
//
//  �t�H�O����(fog.h)
//								����F���n�O��
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

	static void SetFogLinear(float Start,float End);	//�͈͎w��Ńt�H�O���쐬
	static void SetFogDensity(float Density);			//���x�w��Ńt�H�O���쐬

	static void FinishFog();	//�t�H�O���I��
private:

};

#endif 