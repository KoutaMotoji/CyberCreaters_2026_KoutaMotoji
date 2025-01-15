//===============================================================================
//
//  C++�g����2D(object3D.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _BOSS_BULLET_H_
#define _BOSS_BULLET_H_

#include "main.h"
#include "bullet3D.h"

class CBossBullet :public CBullet3D
{
public:
	CBossBullet();				//�R���X�g���N�^
	~CBossBullet()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CBossBullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float Radius, float EffectSize);
protected:
	bool MeshCollision()override;

private:

};

#endif