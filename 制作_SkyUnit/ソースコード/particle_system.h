//===============================================================================
//
//  C++�g����3D(particle_system.h)
//								����F���n�O��
// 
//===============================================================================

#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "main.h"
#include "object.h"
#include "particle_behavior.h"

using namespace std;

/// <summary>
/// �p�[�e�B�N���V�X�e���̃N���X
/// </summary>
class CParticleSystem : public CObject
{
private:
	D3DXVECTOR3 m_pos;	//�ʒu
	int m_nNum;			//�p�[�e�B�N����
	float m_fGap;		//���S�ƊO���̃M���b�v
	CEmitte_Type* m_pType;
public:
	CParticleSystem() : m_pos({ 0.0f,0.0f,0.0f }), m_nNum(0), m_fGap(0.0f) { m_pType = nullptr; }
	~CParticleSystem() {}
	void Init()override {};		//������
	void Uninit()override 
	{
		m_pType->Uninit();
		delete m_pType;
		m_pType = nullptr;
	};		//�I��
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
	};		//�X�V
	void Draw()override {};		//�`��

	void SetOption(CEmitte_Type* TypeBeh) { m_pType = TypeBeh; }
};


#endif