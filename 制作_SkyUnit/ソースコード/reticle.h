//===============================================================================
//
//  C++�g����2D(reticle.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _RETICLE_H_
#define _RETICLE_H_

#include "main.h"
#include "billboard.h"

static constexpr float Poly_Size = 25.0f;

class CReticle :public CBillboard
{
public:
	CReticle();				//�R���X�g���N�^
	~CReticle()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CReticle* Create(D3DXVECTOR3 pos);
private:
};

#endif