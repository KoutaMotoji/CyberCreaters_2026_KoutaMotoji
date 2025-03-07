//===============================================================================
//
//  �Q�[���I�[�o�[�V�[���pUI(go_UI.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _GAMEOVERUI_H_
#define _GAMEOVERUI_H_

#include "main.h"
#include "object2D.h"

class CGameOverUI : CObject2D
{
public:
	CGameOverUI(int nPriority = 5);			//�R���X�g���N�^
	~CGameOverUI()override;		//�f�X�g���N�^
	void Init()override;					//������
	void Uninit()override;					//�I��
	void Update()override;					//�X�V
	void Draw()override;					//�`��

	static CGameOverUI* Create(D3DXVECTOR3 pos,bool bSwitch);
	void ThisDelete();
private:
	int m_TexIdx[2];
};


#endif