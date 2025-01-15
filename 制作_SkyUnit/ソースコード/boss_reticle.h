//===============================================================================
//
//  C++�g����2D(object3D.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _BOSS_RETICLE_H_
#define _BOSS_RETICLE_H_

#include "main.h"
#include "billboard.h"

class CBossReticle :public CBillboard
{
public:
	CBossReticle(int nPriority = 4);				//�R���X�g���N�^
	~CBossReticle()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CBossReticle* Create(D3DXVECTOR3 pos,float Radius,int nLife,float Rotate);
	bool GetLifeState() { return m_nLife <= 0; }
private:
	int m_nLife;
	float m_RotateSpeed;
	float Poly_MinutSize;
};

#endif