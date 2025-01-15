//===============================================================================
//
//  C++�g����3D(go_player.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _GO_PLAYERX_H_
#define _GO_PLAYERX_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"
#include "reticle.h"

#include "manager.h"


#define MAX_MODELPARTS	(20)

#define MAX_PARTS	(20)
#define MAX_KEYSET	(20)
#define MAX_MOTION	(7)

class CShadow;

class CGameOverPlayer :public CObject
{
public:
	CGameOverPlayer(int nPriority = 2);					//�R���X�g���N�^
	~CGameOverPlayer()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��

	void AddMove(D3DXVECTOR3 move) { m_move += move; }
	static CGameOverPlayer* Create(D3DXVECTOR3 pos);

	D3DXVECTOR3 GetPos() { return m_pos; };

	CShadow* m_pShadow;

private:
	D3DXVECTOR3 m_pos, m_rot, m_size;	//���W�E��]�E�傫��
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_OldPos;			//�ߋ��̈ʒu
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X
	void FloorCollision();			//���Ƃ̓����蔻��

	static const float MOVE_SPEED;			//�ړ��X�s�[�h�̐ݒ�

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	void ModelDataLoad();

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//���f���p�[�c��

	//=====================			�N�I�[�^�j�I���p		====================================
	D3DXMATRIX m_mtxRot;		//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION m_quat;		//�N�I�[�^�j�I��
	D3DXVECTOR3 m_vecAxis;		//��]���̃x�N�g��
	float m_fValueRot;			//��]��
};

#endif