//===============================================================================
//
//  C++�g����2D(particle.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "object2D.h"


class CParticle :public CObject2D
{
public:
	CParticle();					//�R���X�g���N�^
	~CParticle()override;			//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static void Load();
	static void Unload();
	static CParticle* Create(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9 m_pTextureTemp;
	static const float PARTICLE_HEIGHT;
	static const float PARTICLE_WIDTH;
};



#endif