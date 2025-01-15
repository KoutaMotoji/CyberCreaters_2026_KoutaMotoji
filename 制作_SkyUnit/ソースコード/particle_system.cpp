//===============================================================================
//
//  C++使った3D(particle_system.cpp)
//								制作：元地弘汰
// 
//===============================================================================

#include "particle3D.h"
#include "particle_system.h"


//==========================================================================================================================
// 継続してエフェクトを出すクラス群

/// <summary>
/// もし射出方法が継続の場合のインスタンスを渡す処理
/// </summary>
CEmitte_Loop* CEmitte_Loop::Instance(CEmitte_Form* Beh, int nLoopTime, int MaxNum)
{
	CEmitte_Loop* pLoop = new CEmitte_Loop();

	pLoop->m_LoopTime = nLoopTime;
	pLoop->m_nRestNum = MaxNum;
	pLoop->m_pForm = Beh;
	return pLoop;
}

//一定間隔でパーティクルを放出する
void CEmitte_Loop::SetEmitte()
{
	if (m_nTimer == m_LoopTime)
	{
		m_nTimer = 0;
		m_pForm->DoEmitte();
		CEmitte_Type::SetEmitte();
	}
	else
	{
		++m_nTimer;
	}
}

//==========================================================================================================================
// 爆発的にエフェクトを出すクラス群

/// <summary>
/// もし射出方法が爆発の場合のインスタンスを渡す処理
/// </summary>
CEmitte_Bomb* CEmitte_Bomb::Instance(CEmitte_Form* Beh, int MaxNum)
{
	CEmitte_Bomb* pBomb = new CEmitte_Bomb();

	pBomb->m_nRestNum = MaxNum;
	pBomb->m_pForm = Beh;
	return pBomb;
}

//一気にパーティクルを放出する
void CEmitte_Bomb::SetEmitte()
{
	int MAX = m_nRestNum;
	for (int i = 0; i < MAX; ++i)
	{	//指定の個数分一気にパーティクルを生成
		m_pForm->DoEmitte();
		CEmitte_Type::SetEmitte();
	}
}

//==========================================================================================================================

//==========================================================================================================================
//パーティクルの発射形状を管理するクラス群の処理

//************************************************
//各形状クラスのインスタンス
CEmitte_Line* CEmitte_Line::Instance(D3DXVECTOR3 dir, int MaxValue, int MinValue)
{
	CEmitte_Line* pLine = new CEmitte_Line();
	pLine->m_direction = dir;
	pLine->m_MaxValue = MaxValue;
	pLine->m_MinValue = MinValue;
	return pLine;
}

CEmitte_Sphere* CEmitte_Sphere::Instance(int MaxValue, int MinValue)
{
	CEmitte_Sphere* pSphere = new CEmitte_Sphere();
	pSphere->m_MaxValue = MaxValue;
	pSphere->m_MinValue = MinValue;
	return pSphere;
}

CEmitte_Cone* CEmitte_Cone::Instance(D3DXVECTOR3 dir,float Radius, int MaxValue, int MinValue)
{
	CEmitte_Cone* pCone = new CEmitte_Cone();
	pCone->m_direction = dir;
	pCone->m_fRadius = Radius;
	pCone->m_MaxValue = MaxValue;
	pCone->m_MinValue = MinValue;
	return pCone;
}
//************************************************

void CEmitte_Line::DoEmitte()
{
	//乱数生成
	std::random_device rnd;				// 非決定的な乱数生成器でシード生成機を生成
	std::mt19937 mt(rnd());				//  メルセンヌツイスターの32ビット版、引数は初期シード
	std::uniform_int_distribution<> rand_x(m_MinValue, m_MaxValue);     // [-1200, 1200] 範囲の一様乱数
	
}

//==========================================================================================================================
