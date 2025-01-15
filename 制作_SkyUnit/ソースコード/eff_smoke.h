//===============================================================================
//
//  C++�g����2D(eff_exolosion.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _EFF_SMOKE_H_
#define _EFF_SMOKE_H_

#include "main.h"
#include "billboard.h"

class CEffSmoke :public CBillboard
{
public:
	CEffSmoke(int nPriority = 1);	//�R���X�g���N�^
	~CEffSmoke()override;			//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CEffSmoke* Create(D3DXVECTOR3 pos, float Radius,int nLife);

private:
	D3DXVECTOR3 SetRandValue();
	D3DXVECTOR3 m_moveValue;
	float m_fRotValue;
	
	int m_nLife;
	int m_nTime;
};

#endif