//===============================================================================
//
//  C++�g����2D(t_starter.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _T_STARTER_H_
#define _T_STARTER_H_

#include "main.h"
#include "object.h"
#include "modelparts.h"

#include "manager.h"


static constexpr int MAX_MODELPARTS = 11;

static constexpr int MAX_PARTS = 11;
static constexpr int MAX_KEYSET =11;
static constexpr int MAX_MOTION = 3;

class CStarter :public CObject
{
public:
	CStarter();					//�R���X�g���N�^
	~CStarter()override;		//�f�X�g���N�^
	void Init()override;		//������
	void Uninit()override;		//�I��
	void Update()override;		//�X�V
	void Draw()override;		//�`��
	static CStarter* Create(D3DXVECTOR3 pos);

	void SetStart();
private:
	D3DXVECTOR3 m_pos, m_rot, m_size;	//���W�E��]�E�傫��
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X

	CModelParts* m_apModelParts[MAX_MODELPARTS];

	char* m_pModelFileName[MAX_MODELPARTS];
	int m_ModelParts;	//���f���p�[�c��
	int m_CurKey;		//���݂̃L�[�ԍ�
	int m_CurMotion;	//���݂̃��[�V�����ԍ�
	int m_NextMotion;	//���݂̃��[�V�����ԍ�
	int m_KeySetNum;	//�L�[�Z�b�g��
	int m_MotionNum;	//���[�V�����ő吔
	int m_NowFrame;		//���݂̃t���[��

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

	enum
	{
		MOTION_NORMAL = 0,
		MOTION_START,
		MOTION_USED
	};
};

#endif