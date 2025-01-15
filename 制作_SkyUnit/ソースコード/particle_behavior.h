//===============================================================================
//
//  C++使った3D(particle_system.h)
//								制作：元地弘汰
// 
//===============================================================================

#ifndef _PARTICLE_BEHAVIOR_H_
#define _PARTICLE_BEHAVIOR_H_

#include "main.h"

//==========================================================================================================================

class CParticle_Looks
{
public:
	CParticle_Looks* Instance(D3DXVECTOR3 pos, float Radius, float Speed, float Gap, bool SizeDown)
	{
		CParticle_Looks* pLooks = new CParticle_Looks();
		pLooks->m_pos = pos;
		pLooks->m_fRadius = Radius;
		pLooks->m_fSpeed = Speed;
		pLooks->m_fGap = Gap;
		pLooks->m_bSizeDown = SizeDown;
		return pLooks;
	}
private:
	D3DXVECTOR3 m_pos;
	float m_fRadius;
	float m_fSpeed;
	float m_fGap;
	bool m_bSizeDown;
};

//==========================================================================================================================
/// <summary>
/// パーティクルの発射形状を管理するクラス群
/// </summary>

class CEmitte_Form
{	//発射形状を管理する親クラス
public:
	CEmitte_Form() : m_MaxValue(0), m_MinValue(0), m_col({0.0f,0.0f,0.0f,0.0f}) {}
	virtual ~CEmitte_Form() = default;
	virtual void DoEmitte() = 0;
protected:
	int m_MaxValue, m_MinValue;
	D3DXCOLOR m_col;
	D3DXVECTOR3 m_pos;
};

class CEmitte_Line : public CEmitte_Form
{	//直線上にパーティクルを射出するタイプ
public:
	CEmitte_Line() : m_direction({ 0.0f,0.0f,0.0f }) {}
	void DoEmitte()override;
	static CEmitte_Line* Instance( D3DXVECTOR3 dir, int MaxValue, int MinValue);
private:
	D3DXVECTOR3 m_direction;
};

class CEmitte_Sphere : public CEmitte_Form
{	//球形(全方向)にパーティクルを射出するタイプ
public:
	void DoEmitte()override {};
	static CEmitte_Sphere* Instance(int MaxValue, int MinValue);
private:

};

class CEmitte_Cone : public CEmitte_Form
{	//円錐状にパーティクルを射出するタイプ
public:
	CEmitte_Cone() : m_direction({ 0.0f,0.0f,0.0f }), m_fRadius(0.0f) {}
	void DoEmitte()override {};
	static CEmitte_Cone* Instance(D3DXVECTOR3 dir, float Radius, int MaxValue, int MinValue);
private:
	D3DXVECTOR3 m_direction;
	float m_fRadius;
};

//==========================================================================================================================

//==========================================================================================================================
/// <summary>
/// パーティクルの射出方法を管理するクラス群
/// </summary>

class CEmitte_Type
{	//どういう出方をするのか管理する親クラス
public:
	CEmitte_Type() :m_nRestNum(0) {}
	void Uninit()
	{
		delete m_pForm;
		m_pForm = nullptr;
	}
	virtual ~CEmitte_Type() = default;
	virtual void SetEmitte() { --m_nRestNum; };
	bool RestNumCheck() { return m_nRestNum < 0; }
protected:
	int m_nRestNum;
	CEmitte_Form* m_pForm;
private:

};

class CEmitte_Loop : public CEmitte_Type
{	//継続的に発射する
public:
	CEmitte_Loop() : m_LoopTime(0), m_nTimer(0){};
	void SetEmitte()override;
	static CEmitte_Loop* Instance(CEmitte_Form* Beh, int nLoopTime, int MaxNum);
private:
	int m_LoopTime,m_nTimer;
};

class CEmitte_Bomb : public CEmitte_Type
{	//一瞬で爆発的に発射する
	void SetEmitte()override;
	static CEmitte_Bomb* Instance(CEmitte_Form* Beh, int MaxNum);
};

//==========================================================================================================================

#endif