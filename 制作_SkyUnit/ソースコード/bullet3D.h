//===============================================================================
//
//  C++�g����2D(object3D.h)
//								����F���n�O��
// 
//===============================================================================

#ifndef _BULLET3D_H_
#define _BULLET3D_H_

#include "main.h"
#include "billboard.h"

class CBullet3D :public CBillboard
{
public:
	CBullet3D(int nPriority = 4);				//�R���X�g���N�^
	~CBullet3D()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CBullet3D* Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col,int nLife,float Radius,float EffectSize);
protected:
	virtual bool MeshCollision();
	float Poly_Size;
	float m_EffectSize;
	D3DXVECTOR3 m_move;
	D3DXCOLOR m_col;
	int m_nLife;

private:

};

#endif