//===============================================================================
//
//  C++�g����2D(playerX.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _PLAYERX_H_
#define _PLAYERX_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"
#include "reticle.h"
#include "shadow.h"

#include "manager.h"


class CEnemyBase;
class CCollision;
class CMainUI;

class CPlayerX :public CObject
{
public:
	CPlayerX();					//�R���X�g���N�^
	~CPlayerX()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void SetDamageState();
	void ShootBullet();
	void Draw()override;		//�`��

	bool PMove(float fCamRotZ);
	static CPlayerX* Create(D3DXVECTOR3 pos);
	int GetPLife() { return m_nLife; };

	D3DXVECTOR3 GetPos() { return m_pos; };
	D3DXVECTOR3 GetMove() { return m_move; };
private:

	//���[�V�����p�萔
	static constexpr int MAX_MODELPARTS = 20;
	static constexpr int MAX_PARTS = 20;
	static constexpr int MAX_KEYSET = 20;
	static constexpr int MAX_MOTION = 7;

	//�X�e�[�^�X�p�萔
	static constexpr int MAX_LIFE = 1000;			//�̗�
	static constexpr float MOVE_JET_SPEED = 3.45f;	//�ړ��X�s�[�h�̐ݒ�
	static constexpr float MOVE_ROBO_SPEED = 2.00f;	//�ړ��X�s�[�h�̐ݒ�

	static constexpr float SCROLL_SPEED = 5.5f;		//�����X�N���[���X�s�[�h�̐ݒ�
	static constexpr float RETICLE_VALUE = 8.5f;	//���e�B�N���̈ړ��M���b�v

	D3DXVECTOR3 m_pos,m_rot,m_size;			//���W�E��]�E�傫��
	D3DXVECTOR3 m_move;						//�ړ���
	D3DXVECTOR3 m_OldPos;					//�ߋ��̈ʒu
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxWeaponWorld;			//����̃��[���h�}�g���b�N�X
	D3DXVECTOR3 m_WeaponCollisionPos;		//����̊p�x�����ƂɎZ�o���铖���蔻��̑��Έʒu
	void FloorCollision();					//���Ƃ̓����蔻��

	void SetWeaponRot(D3DXVECTOR2 rot);		//����̕�����ݒ�
	void DamageAdd(int nDmg);				//�_���[�W���Z�A�����m�F
	D3DXVECTOR3 LockOnEnemy();				//�G�̃��b�N�I��
	void AttackCollisionToEnemy();			//�G�ւ̍U������
	void BossAttackCollision();				//�{�X�̍U������

	void DeadCheck();
	float m_fWeaponRadius;
	int m_nLife;			//�̗�

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//���f���p�[�c��
	int m_CurKey;		//���݂̃L�[�ԍ�
	int m_CurMotion;	//���݂̃��[�V�����ԍ�
	int m_NextMotion;	//���݂̃��[�V�����ԍ�
	int m_KeySetNum;	//�L�[�Z�b�g��
	int m_MotionNum;	//���[�V�����ő吔
	int m_NowFrame;		//���݂̃t���[��
	float m_SecZrot;
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

	void TransformSound();

	enum
	{
		MOTION_JET_NUTO = 0,
		MOTION_JET_NOMAL,
		MOTION_TRANS_JET_TO_ROBO,
		MOTION_ROBO_NUTO,
		MOTION_TRANS_ROBO_TO_JET,
		MOTION_ROBO_SHOT,
		MOTION_ROBO_SLASH
	};

	bool m_bBlend;
	bool m_bMotion;
	bool m_bTransformed;
	bool m_bAttack;
	bool TestUseMeshCollision();
	CReticle* m_pReticle;
	void ReticleController();
	D3DXVECTOR3 CameraPosDigit();
	D3DXVECTOR3 RifleMtxSet();
	bool m_bDamaged;
	int m_DamageTime;
	bool MeshObstacle();
	void GetItem();
	bool CheckLockonBlocking();
	bool PushRSholder();

	bool CheckToObs(CEnemyBase* pEnemy);
	CShadow* m_pShadow;
	CMainUI* m_pMainUI;

	//========================			�N�I�[�^�j�I���p		====================================
	D3DXMATRIX m_mtxRot;		//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION m_quat;		//�N�I�[�^�j�I��
	D3DXVECTOR3 m_vecAxis;		//��]���̃x�N�g��
	float m_fValueRot;			//��]��
};

#endif