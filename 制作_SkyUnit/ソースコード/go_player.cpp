//===============================================================================
//
//  C++�g����2D(go_player.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "go_player.h"
#include "shadow.h"

const float CGameOverPlayer::MOVE_SPEED = 0.55f;


//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CGameOverPlayer::CGameOverPlayer(int nPriority) : CObject(nPriority)
{
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i] = nullptr;
	}
}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CGameOverPlayer::~CGameOverPlayer()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CGameOverPlayer::Init()
{
	ModelDataLoad();
	CObject::SetType(TYPE_3D_PLAYER);
	m_pShadow = CShadow::Create(m_pos, 50);

	//CManager::GetInstance()->GetCamera()->SetRotz(D3DX_PI);
}

//==========================================================================================
//�I������
//==========================================================================================
void CGameOverPlayer::Uninit()
{
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i]->Uninit();
	}
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CGameOverPlayer::Update()
{
	m_pShadow->SetShadowGround(m_pos);

	m_pos += m_move;
	//�ړ��ʂ��X�V
	m_move.x += (0.0f - m_move.x) * 0.017f;
	m_move.y += (0.0f - m_move.y) * 0.017f;
	m_move.z += (0.0f - m_move.z) * 0.017f;
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CGameOverPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�傫���𔽉f
	D3DXMatrixScaling(&mtxSize,
		m_size.y,
		m_size.x,
		m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxSize);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtxWorld);
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i]->Draw();
	}
}

//==========================================================================================
//��������
//==========================================================================================
CGameOverPlayer* CGameOverPlayer::Create(D3DXVECTOR3 pos)
{
	CGameOverPlayer* player = new CGameOverPlayer;
	player->Init();

	player->m_pos = pos;
	player->m_move = { 0.0f,0.0f,0.0f };
	player->m_rot = { 0.0f,0.0f,0.0f };
	player->m_size = { 1.0f,1.0f,1.0f };
	player->m_OldPos = pos;
	return player;
}

//==========================================================================================
//�������蔻��
//==========================================================================================
void CGameOverPlayer::FloorCollision()
{

}


//==========================================================================================
//���[�V�������t�@�C������ǂݍ���
//==========================================================================================
void CGameOverPlayer::ModelDataLoad()
{
	char LoadData[128];
	char ModelPath[128];

	int nNumModel = 0;
	FILE* pFile;
	int nCnt = 0;

	int nMotionCnt = 0;
	int nKeySet = 0;
	int nKey = 0;
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	int nFilenameCnt = 0;
	int nParent = 0;
	int nIndex = 0;
	int nModelCnt = 0;

	pFile = fopen("data\\TEXT\\motion_gameover_player.txt", "r");

	if (pFile != nullptr)
	{
		while (1)
		{
			fscanf(pFile, "%s", LoadData);

			if (!strcmp(LoadData, "END_SCRIPT"))	// �t�@�C���̍Ō�
			{
				fclose(pFile);
				break;
			}

			if (LoadData[0] == '#')		// ������΂�
			{
				continue;
			}

			//���f���̓ǂݍ���
			if (!strcmp(LoadData, "NUM_MODEL"))
			{
				fscanf(pFile, "%s", LoadData);
				fscanf(pFile, "%d", &m_ModelParts);
			}
			//���f���̃t�@�C�����ǂݍ���
			if (!strcmp(LoadData, "MODEL_FILENAME"))
			{
				fscanf(pFile, "%s", LoadData);

				fscanf(pFile, "%s", ModelPath);

				m_pModelFileName[nFilenameCnt] = ModelPath;

				m_apModelParts[nFilenameCnt] = CModelParts::Create(pos, m_pModelFileName[nFilenameCnt]);

				++nFilenameCnt;
			}
			//�L�����N�^�[�̐ݒ�̓ǂݍ��݊J�n
			if (!strcmp(LoadData, "CHARACTERSET"))
			{
				while (1)
				{
					fscanf(pFile, "%s", LoadData);

					if (!strcmp(LoadData, "END_CHARACTERSET"))//�ǂݍ��݂��I��
					{
						break;
					}
					else if (!strcmp(LoadData, "PARTSSET"))
					{
						while (1)
						{
							fscanf(pFile, "%s", LoadData);

							if (LoadData[0] == '#')
							{//������΂�
								fgets(LoadData, 100, pFile);
								continue;
							}

							if (!strcmp(LoadData, "END_PARTSSET"))
							{
								//�ǂݍ��݂��I��
								break;
							}
							//�e�p�[�c�̃��[�V����pos�l
							else if (!strcmp(LoadData, "INDEX"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &nIndex);
							}

							//�e�p�[�c�̃��[�V����rot�l
							else if (!strcmp(LoadData, "PARENT"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &nParent);
							}
							//�e�p�[�c�̃��[�V����pos�l
							else if (!strcmp(LoadData, "POS"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%f", &pos.x);
								fscanf(pFile, "%f", &pos.y);
								fscanf(pFile, "%f", &pos.z);
							}

							//�e�p�[�c�̃��[�V����rot�l
							else if (!strcmp(LoadData, "ROT"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%f", &rot.x);
								fscanf(pFile, "%f", &rot.y);
								fscanf(pFile, "%f", &rot.z);
							}
						}
						m_apModelParts[nModelCnt]->SetPos(pos);
						m_apModelParts[nModelCnt]->SetRot(rot);

						m_apModelParts[nModelCnt]->SetDefault();
						m_apModelParts[nModelCnt]->SetIndex(nIndex);
						if (nParent != -1)
						{
							m_apModelParts[nModelCnt]->SetParent(m_apModelParts[nParent]);
						}
						++nModelCnt;
					}
				}
			}
		}
	}
	else
	{
		assert(pFile == nullptr);
	}
}