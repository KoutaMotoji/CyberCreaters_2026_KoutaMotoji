//===============================================================================
//
//  C++�g����3D(t_anim_border.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _TITLEANIM_BORDER_H_
#define _TITLEANIM_BORDER_H_

#include "main.h"
#include "object2D.h"


class CTAnimBorder :public CObject2D
{
public:
	CTAnimBorder(int nPriority = SET_PRIORITY - 2);		//�R���X�g���N�^
	~CTAnimBorder()override;				//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��

	static CTAnimBorder* Create(int type);

protected:
	virtual void SetPosToAnim() {};
private:
};

class CTAnimBorderU :public CTAnimBorder
{
public:
	void Init()override;		//������
	//�`��
protected:
	void SetPosToAnim()override;
};

class CTAnimBorderD :public CTAnimBorder
{
public:
	void Init()override;		//������
	//�`��
protected:
	void SetPosToAnim()override;
};
#endif