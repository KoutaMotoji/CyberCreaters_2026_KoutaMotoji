//===============================================================================
//
//  C++�g����2D(eff_exolosion.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _EFF_EXPLOSION_H_
#define _EFF_EXPLOSION_H_

#include "main.h"
#include "billboard.h"

class CEffExplosion :public CBillboard
{
public:
	CEffExplosion(int nPriority = 4);	//�R���X�g���N�^
	~CEffExplosion()override;			//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CEffExplosion* Create(D3DXVECTOR3 pos);
	static CEffExplosion* Create(D3DXVECTOR3 pos,float Radius);

private:
	int m_nTime;
};

#endif