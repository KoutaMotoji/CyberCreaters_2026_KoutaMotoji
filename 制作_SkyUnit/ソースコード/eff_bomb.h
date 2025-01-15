//===============================================================================
//
//  C++�g����2D(eff_bomb.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _EFF_BOMB_H_
#define _EFF_BOMB_H_

#include "main.h"
#include "billboard.h"

class CEffBomb :public CBillboard
{
public:
	CEffBomb(int nPriority = 5);				//�R���X�g���N�^
	~CEffBomb()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CEffBomb* Create(D3DXVECTOR3 pos);
	static CEffBomb* Create(D3DXVECTOR3 pos, float Radius);

private:
	int m_nTime;
};

#endif