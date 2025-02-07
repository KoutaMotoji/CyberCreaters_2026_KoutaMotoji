//===============================================================================
//
//  C++�g����2D(t_player.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _R_BOSSX_H_
#define _R_BOSSX_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"

#include "manager.h"

static constexpr int RB_MAX_MODELPARTS = 21;

static constexpr int RB_MAX_PARTS = 21;
static constexpr int RB_MAX_KEYSET = 21;
static constexpr int RB_MAX_MOTION = 7;

class CResultBoss :public CObject
{
public:
	CResultBoss(int nPriority = 0);					//�R���X�g���N�^
	~CResultBoss()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��

	void AddMove(D3DXVECTOR3 move) { m_move += move; }
	static CResultBoss* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	D3DXVECTOR3 GetPos() { return m_pos; };
private:
	D3DXVECTOR3 m_pos, m_rot, m_size;	//���W�E��]�E�傫��
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_OldPos;			//�ߋ��̈ʒu
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X

	static const float MOVE_SPEED;			//�ړ��X�s�[�h�̐ݒ�

	CModelParts* m_apModelParts[RB_MAX_MODELPARTS];

	void ModelDataLoad();

	char* m_pModelFileName[RB_MAX_MODELPARTS];
	int m_ModelParts;	//���f���p�[�c��

	//=====================			�N�I�[�^�j�I���p		====================================
	D3DXMATRIX m_mtxRot;		//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION m_quat;		//�N�I�[�^�j�I��
	D3DXVECTOR3 m_vecAxis;		//��]���̃x�N�g��
	float m_fValueRot;			//��]��
};

#endif