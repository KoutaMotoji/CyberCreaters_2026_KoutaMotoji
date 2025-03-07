//===============================================================================
//
//  �A�C�e���̏���(3D_Item.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _3DITEM_H_
#define _3DITEM_H_

#include "main.h"
#include "objectX.h"

static constexpr int MIN_CNT = 40;

class C3DItem :public CObjectX
{
public:
	C3DItem() :m_nLestTime(0), m_bGot(false) {};	//�R���X�g���N�^
	~C3DItem()override = default;					//�f�X�g���N�^

	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��

	void GotThisItem() { m_bGot = true; }
	bool GetItemUse() { return m_bGot; }
	static C3DItem* Create(D3DXVECTOR3 pos);
private:
	bool m_bGot;
	int m_nLestTime;

	void GotAnimation();
};

#endif