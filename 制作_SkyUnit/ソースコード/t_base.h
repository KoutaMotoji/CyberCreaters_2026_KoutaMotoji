//===============================================================================
//
//  C++�g����2D(enemy_basic.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _T_BASE_H_
#define _T_BASE_H_

#include "main.h"
#include "objectX.h"


class CTitleBase :public CObjectX
{
public:

	CTitleBase();				//�R���X�g���N�^
	~CTitleBase()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CTitleBase* Create(D3DXVECTOR3 pos);
protected:

private:

};



#endif