//===============================================================================
//
//  C++使った3D(particle_system.h)
//								制作：元地弘汰
// 
//===============================================================================

#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "main.h"
#include "object.h"
#include "particle_behavior.h"

using namespace std;

/// <summary>
/// パーティクルシステムのクラス
/// </summary>
class CParticleSystem : public CObject
{
private:
	D3DXVECTOR3 m_pos;	//位置
	int m_nNum;			//パーティクル数
	float m_fGap;		//中心と外側のギャップ
	CEmitte_Type* m_pType;
public:
	CParticleSystem() : m_pos({ 0.0f,0.0f,0.0f }), m_nNum(0), m_fGap(0.0f) { m_pType = nullptr; }
	~CParticleSystem() {}
	void Init()override {};		//初期化
	void Uninit()override 
	{
		m_pType->Uninit();
		delete m_pType;
		m_pType = nullptr;
	};		//終了
	void Update()override 
	{
		if (m_pType != nullptr)
		{
			m_pType->SetEmitte();
			if (m_pType->RestNumCheck())
			{
				Uninit();
				return;
			}
		}
	};		//更新
	void Draw()override {};		//描画

	void SetOption(CEmitte_Type* TypeBeh) { m_pType = TypeBeh; }
};


#endif