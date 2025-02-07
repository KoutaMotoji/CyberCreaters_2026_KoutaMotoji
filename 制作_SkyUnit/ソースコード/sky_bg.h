//===============================================================================
//
//  C++�g����3D(field.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _SKY_BG_H_
#define _SKY_BG_H_

#include "main.h"
#include "object3D.h"


class CSkyBg :public CObject3D
{
public:
	CSkyBg();					//�R���X�g���N�^
	~CSkyBg()override;			//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��

	static CSkyBg* Create(D3DXVECTOR3 pos);

private:
	static constexpr float FLOOR_SIZE = 15000.0f;
};



#endif