//===============================================================================
//
//  ���U���g�V�[���p�v���C���[����(t_player.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _R_PLAYERX_H_
#define _R_PLAYERX_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"

#include "manager.h"

class CResultPlayer :public CObject
{
public:
	CResultPlayer(int nPriority = 0);					//�R���X�g���N�^
	~CResultPlayer()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��

	inline void AddMove(D3DXVECTOR3 move) { m_move += move; }
	static CResultPlayer* Create(D3DXVECTOR3 pos);

	inline D3DXVECTOR3 GetPos() { return m_pos; };
private:
	static constexpr int MAX_MODELPARTS = 20;

	static constexpr int MAX_PARTS = 20;
	static constexpr int MAX_KEYSET = 20;
	static constexpr int MAX_MOTION = 7;

	D3DXVECTOR3 m_pos, m_rot, m_size;	//���W�E��]�E�傫��
	D3DXVECTOR3 m_move;				//�ړ���
	D3DXVECTOR3 m_OldPos;			//�ߋ��̈ʒu
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X

	static const float MOVE_SPEED;			//�ړ��X�s�[�h�̐ݒ�

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	void ModelDataLoad();

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//���f���p�[�c��
	int m_CurKey;		//���݂̃L�[�ԍ�
	int m_CurMotion;	//���݂̃��[�V�����ԍ�
	int m_NextMotion;	//���݂̃��[�V�����ԍ�
	int m_KeySetNum;	//�L�[�Z�b�g��
	int m_MotionNum;	//���[�V�����ő吔
	int m_NowFrame;		//���݂̃t���[��
	bool m_bBlend;

	struct Key
	{
		//�ʒu
		D3DXVECTOR3 pos;
		//����
		D3DXVECTOR3 rot;
	};
	struct KeySet
	{

		int nFrame;
		Key aKey[MAX_PARTS];
	};
	struct Motion
	{
		int bLoop;
		int nKeyNum;
		KeySet aKetSet[MAX_KEYSET];
	};
	Motion m_aMotion[MAX_MOTION];

	void MotionInit();
	void MotionDataLoad();
	void SetNextMotion(int nNextMotionNum);
	void SetNextKey();
	bool MotionBlending();

	//=====================			�N�I�[�^�j�I���p		====================================
	D3DXMATRIX m_mtxRot;		//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION m_quat;		//�N�I�[�^�j�I��
	D3DXVECTOR3 m_vecAxis;		//��]���̃x�N�g��
	float m_fValueRot;			//��]��
};

#endif