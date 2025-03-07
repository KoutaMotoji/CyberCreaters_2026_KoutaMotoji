//===============================================================================
//
//		 �G�̃��b�N�I������(lockon_enemy.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _LOCKON_ENEMY_H_
#define _LOCKON_ENEMY_H_

#include "main.h"
#include "billboard.h"


static const float POLY_RADIUS = 120.0f;

class CLockonEnemy :public CBillboard
{
public:
	CLockonEnemy();				//�R���X�g���N�^
	~CLockonEnemy()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CLockonEnemy* Create(D3DXVECTOR3 pos);

	void SetThisPos(D3DXVECTOR3 pos) { CBillboard::SetPos(pos); }
protected:

private:

};

#endif