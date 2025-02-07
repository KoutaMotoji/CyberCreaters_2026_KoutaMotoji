//===============================================================================
//
//  C++�g����2D(playerX.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "playerX.h"
#include "player_UI.h"

#include "3D_Item.h"
#include "particle3D.h"
#include "bullet3D.h"
#include "enemy_base.h"
#include "boss_bullet.h"
#include "mesh_Boss_Terra.h"
#include "mesh_ground.h" 
#include "mesh_obstacle.h"
#include "mesh_cylinder.h"
#include "eff_bomb.h"
#include "game.h"
#include "collision.h"

namespace
{
	float Damage_Ratio = 0.2f;
};

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CPlayerX::CPlayerX():m_nLife(1000),m_fWeaponRadius(25), 
					m_bMotion(false), m_SecZrot(0.8f), 
					m_bTransformed(false), m_bDamaged(false),
					m_DamageTime(0), m_bBlend(false), m_bAttack(false)
{
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i] = nullptr;
	}
	m_pShadow = nullptr;
}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CPlayerX::~CPlayerX()
{
	
}

//==========================================================================================
//����������
//==========================================================================================
void CPlayerX::Init()
{
	MotionInit();
	m_pReticle = CReticle::Create({ 0.0f,0.0f,500.0f });
	CObject::SetType(TYPE_3D_PLAYER);
	m_pShadow = CShadow::Create({ 0.0f,0.0f,0.0f });
	CGaugeLife::Create(MAX_LIFE);
	m_pMainUI = CMainUI::Create();
}

//==========================================================================================
//�I������
//==========================================================================================
void CPlayerX::Uninit()
{
	for (int i = 0; i < MAX_MODELPARTS; ++i)
	{
		m_apModelParts[i]->Uninit();
	}
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CPlayerX::Update()
{
	D3DXVECTOR3 CameraPos;
	m_pShadow->SetShadowGround(m_pos);
	ReticleController();
	if (!MotionBlending())
	{
		SetNextKey();
	}

	FloorCollision();
	PMove(CameraPos.z);
	if (!m_bMotion)
	{
		if (CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_X) == true||
			CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_R) == true)
		{
			if (!m_bTransformed)
			{
				SetNextMotion(MOTION_TRANS_JET_TO_ROBO);
			}
			else
			{
				SetNextMotion(MOTION_TRANS_ROBO_TO_JET);
			}
			m_bTransformed = !m_bTransformed;
			m_bMotion = true;
		}
	}
	GetItem();

	if (MeshObstacle() )
	{
		SetDamageState();
	}
	if (!TestUseMeshCollision())
	{
		m_pReticle->AddPos({m_move.x,m_move.y,0.0f});
	}
	if (m_bDamaged)
	{
		++m_DamageTime;
		m_rot.x += 0.2f;
		m_rot.y += 0.3f;
		if (m_DamageTime > 100)
		{
			m_rot = {0.0f,D3DX_PI,0.0f};
			m_bDamaged = false;
		}
	}

	ShootBullet();

	m_move.z += SCROLL_SPEED;
	m_pReticle->SetPos({ m_pReticle->GetPos().x,m_pReticle->GetPos().y,m_pos.z + 500 });

	m_pos += m_move;
	if (PushRSholder())
	{
		m_pos -= {m_move.x, m_move.y, 0.0f};
	}
	//�ړ��ʂ��X�V
	m_move.x += (0.0f - m_move.x) * 0.14f;
	m_move.y += (0.0f - m_move.y) * 0.14f;
	m_move.z += (0.0f - m_move.z) * 0.17f;

	CameraPos = CameraPosDigit();

	CManager::GetInstance()->GetCamera()->SetPlayerPos(CameraPos);
}

void CPlayerX::SetDamageState()
{
	if (!m_bDamaged)
	{
		m_bDamaged = true;
		CManager::GetInstance()->GetCamera()->SetShake(20, 40);
		DamageAdd(1000 * Damage_Ratio);
		m_DamageTime = 0;
	}
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CPlayerX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	//�N�I�[�^�j�I�����쐬
	D3DXQuaternionRotationAxis(
		&m_quat,
		&m_vecAxis,
		m_fValueRot);

	//�N�I�[�^�j�I�������]�}�g���b�N�X���쐬
	D3DXMatrixRotationQuaternion(
		&mtxRot,
		&m_quat);
	m_mtxRot = mtxRot;		//��]�}�g���b�N�X��ۑ�

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
		&m_mtxRot);
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
		if (m_bDamaged)
		{
			m_apModelParts[i]->Draw(0.3f);
		}
		else
		{
			m_apModelParts[i]->Draw();
		}
	}
	m_pReticle->Draw();
}

//==========================================================================================
//��������
//==========================================================================================
CPlayerX* CPlayerX::Create(D3DXVECTOR3 pos)
{
	CPlayerX* player = new CPlayerX;
	player->Init();

	player->m_pos = pos;
	player->m_move = { 0.0f,0.0f,0.0f };
	player->m_rot = { 0.0f,D3DX_PI,0.0f };
	player->m_size = {1.0f,1.0f,1.0f};
	player->m_OldPos = pos;
	return player;
}

//==========================================================================================
//�ړ�����
//==========================================================================================
bool CPlayerX::PMove(float fCamRotZ)
{
	if (!m_bTransformed)
	{
		m_move += {CManager::GetInstance()->GetJoypad()->GetJoyStickVecL().x * MOVE_JET_SPEED, CManager::GetInstance()->GetJoypad()->GetJoyStickVecL().y * MOVE_JET_SPEED, 0.0f};
		m_vecAxis = { m_move.y,0.0f,m_move.x };
	}
	else
	{
		m_move += {CManager::GetInstance()->GetJoypad()->GetJoyStickVecL().x * MOVE_ROBO_SPEED, CManager::GetInstance()->GetJoypad()->GetJoyStickVecL().y * MOVE_ROBO_SPEED, 0.0f};
		m_vecAxis = { m_move.y,m_move.x,0.0f };
	}

	D3DXVec3Normalize(&m_vecAxis, &m_vecAxis);

	m_fValueRot = (2 * sqrtf((m_move.x * m_move.x) + (m_move.y * m_move.y)) * 10) / (120 * D3DX_PI);
	m_pReticle->SetPos({ m_pos.x + m_move.x * RETICLE_VALUE,m_pos.y + m_move.y * RETICLE_VALUE, m_pos.z + 500 });

	return true;
}


//==========================================================================================
// �v���C���[�̈ړ���������
//==========================================================================================
void CPlayerX::FloorCollision()
{
	if (m_pos.y < -1000)
	{
		m_pos.y = -1000;
	}
	else if (m_pos.y > 1000)
	{
		m_pos.y = 1000;
	}
	if (m_pos.x < -1200)
	{
		m_pos.x = -1200;
	}
	else if (m_pos.x > 1200)
	{
		m_pos.x = 1200;
	}

	if (m_pReticle->GetPos().y < m_pos.y -300)
	{
		m_pReticle->SetPos({ m_pReticle->GetPos().x, m_pos.y - 300 ,m_pReticle->GetPos().z });
	}
	else if (m_pReticle->GetPos().y > m_pos.y + 300)
	{
		m_pReticle->SetPos({ m_pReticle->GetPos().x, m_pos.y + 300 ,m_pReticle->GetPos().z });
	}
	if (m_pReticle->GetPos().x < m_pos.x - 500)
	{
		m_pReticle->SetPos({ m_pos.x - 500, m_pReticle->GetPos().y ,m_pReticle->GetPos().z });
	}
	else if (m_pReticle->GetPos().x > m_pos.x + 500)
	{
		m_pReticle->SetPos({ m_pos.x + 500, m_pReticle->GetPos().y ,m_pReticle->GetPos().z });
	}
}

//==========================================================================================
//�ːi�U���̎��ɕ���̕�����i�s�����ɕϊ�
//==========================================================================================
void CPlayerX::SetWeaponRot(D3DXVECTOR2 rot)
{
	m_rot = { -(abs(atan2f(rot.x ,rot.y))), m_rot.y,0.0f };
}

//==========================================================================================
//���S�`�F�b�N
//==========================================================================================
void CPlayerX::DeadCheck()
{
	if (m_nLife <= 0)
	{
		CManager::GetInstance()->GetFade()->SetFade(CFade::FADE_IN, CScene::MODE_GAMEOVER);
	}
}

//==========================================================================================
//���[�V�����̏���������
//==========================================================================================
void CPlayerX::MotionInit()
{
	MotionDataLoad();

	m_CurMotion = 0;
	m_CurKey = 0;
	m_NowFrame = 0;
}

//==========================================================================================
//���̃��[�V�����Ɉڍs���鏈��
//==========================================================================================
void CPlayerX::SetNextMotion(int nNextMotionNum)
{
	m_NextMotion = nNextMotionNum;
	m_NowFrame = 0;
	m_bBlend = true;
	MotionBlending();
}

//==========================================================================================
//���̃L�[�̃��[�V��������
//==========================================================================================
void CPlayerX::SetNextKey()
{
	//���݂̈ʒu�E�p�x
	D3DXVECTOR3 NowPos = {};
	D3DXVECTOR3 NowRot = {};

	//���̈ʒu�E�p�x
	D3DXVECTOR3 NextPos = {};
	D3DXVECTOR3 NextRot = {};

	//�����̍��������߂�p
	D3DXVECTOR3 DifPos = {};
	D3DXVECTOR3 DifRot = {};

	//�v�Z�p
	D3DXVECTOR3 DigitPos = {};
	D3DXVECTOR3 DigitRot = {};

	int nNowKey = m_CurKey;
	int nNowMotion = m_CurMotion;
	int nNextKey = (m_CurKey + 1) % m_aMotion[nNowMotion].nKeyNum;
	float fRatioFrame = (float)m_NowFrame / (float)m_aMotion[nNowMotion].aKetSet[nNowKey].nFrame;

	for (int nCntParts = 0; nCntParts < MAX_PARTS; ++nCntParts)
	{
		//���݂̌����ƈʒu�̏��
		NowPos = m_aMotion[nNowMotion].aKetSet[nNowKey].aKey[nCntParts].pos;
		NowRot = m_aMotion[nNowMotion].aKetSet[nNowKey].aKey[nCntParts].rot;
		
		//���̃L�[�̏��
		NextPos = m_aMotion[nNowMotion].aKetSet[nNextKey].aKey[nCntParts].pos;
		NextRot = m_aMotion[nNowMotion].aKetSet[nNextKey].aKey[nCntParts].rot;

		//���������߂�
		DifPos = NextPos - NowPos;
		DifRot = NextRot - NowRot;

		//-3.14�`3.14�̊Ԃ𒴂���ꍇ�̏C��
		if (DifRot.x >= D3DX_PI)
		{
			DifRot.x -= D3DX_PI * 2;
		}
		if (DifRot.y >= D3DX_PI)
		{
			DifRot.y -= D3DX_PI * 2;
		}
		if (DifRot.z >= D3DX_PI)
		{
			DifRot.z -= D3DX_PI * 2;
		}
		if (DifRot.x <= -D3DX_PI)
		{
			DifRot.x += D3DX_PI * 2;
		}
		if (DifRot.y <= -D3DX_PI)
		{
			DifRot.y += D3DX_PI * 2;
		}
		if (DifRot.z <= -D3DX_PI)
		{
			DifRot.z += D3DX_PI * 2;
		}

		DigitPos = DifPos * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultPos() + NowPos;
		DigitRot = DifRot * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultRot() + NowRot;

		m_apModelParts[nCntParts]->SetPos(DigitPos);
		m_apModelParts[nCntParts]->SetRot(DigitRot);
	}

	++m_NowFrame;

	if (m_NowFrame >= m_aMotion[nNowMotion].aKetSet[nNowKey].nFrame)
	{
		++m_CurKey;
		m_NowFrame = 0;
		if (m_CurMotion == MOTION_TRANS_JET_TO_ROBO ||
			m_CurMotion == MOTION_TRANS_ROBO_TO_JET)
		{
			TransformSound();
		}
		if (m_CurKey >= m_aMotion[nNowMotion].nKeyNum)
		{
			if (!m_aMotion[nNowMotion].bLoop)
			{	
				if (m_CurMotion == MOTION_TRANS_JET_TO_ROBO ||
					m_CurMotion == MOTION_TRANS_ROBO_TO_JET ||
					m_CurMotion == MOTION_ROBO_SHOT || 
					m_CurMotion == MOTION_ROBO_SLASH)
				{
					
					m_pMainUI->ChangeUI((int)m_bTransformed);
					
					--m_CurKey;
					m_bMotion = false;
				}
				if (m_CurMotion == MOTION_ROBO_SLASH)
				{
					m_bAttack = false;
				}
				if (m_bTransformed)
				{
					SetNextMotion(MOTION_ROBO_NUTO);
				}
				else
				{
					SetNextMotion(MOTION_JET_NUTO);
				}			
			}
			else
			{
				m_CurKey = 0;
			}
		}
	}
}
//==========================================================================================
//���[�V�����؂�ւ����̃u�����h����
//==========================================================================================
bool CPlayerX::MotionBlending()
{
	if (!m_bBlend){
		return false;
	}

	//���݂̈ʒu�E�p�x
	D3DXVECTOR3 NowPos = {};
	D3DXVECTOR3 NowRot = {};

	//���̈ʒu�E�p�x
	D3DXVECTOR3 NextPos = {};
	D3DXVECTOR3 NextRot = {};

	//�����̍��������߂�p
	D3DXVECTOR3 DifPos = {};
	D3DXVECTOR3 DifRot = {};

	//�v�Z�p
	D3DXVECTOR3 DigitPos = {};
	D3DXVECTOR3 DigitRot = {};

	int nLastKey = m_CurKey;
	int nNowMotion = m_CurMotion;

	float fRatioFrame = ((float)m_NowFrame / (float)m_aMotion[m_NextMotion].aKetSet[0].nFrame);

	for (int nCntParts = 0; nCntParts < MAX_PARTS; ++nCntParts)
	{
		//���݂̌����ƈʒu�̏��
		NowPos = m_aMotion[nNowMotion].aKetSet[nLastKey].aKey[nCntParts].pos;
		NowRot = m_aMotion[nNowMotion].aKetSet[nLastKey].aKey[nCntParts].rot;

		//���̃L�[�̏��
		NextPos = m_aMotion[m_NextMotion].aKetSet[0].aKey[nCntParts].pos;
		NextRot = m_aMotion[m_NextMotion].aKetSet[0].aKey[nCntParts].rot;

		//���������߂�
		DifPos = NextPos - NowPos;
		DifRot = NextRot - NowRot;

		//-3.14�`3.14�̊Ԃ𒴂���ꍇ�̏C��
		if (DifRot.x >= D3DX_PI)
		{
			DifRot.x -= D3DX_PI * 2;
		}
		if (DifRot.y >= D3DX_PI)
		{
			DifRot.y -= D3DX_PI * 2;
		}
		if (DifRot.z >= D3DX_PI)
		{
			DifRot.z -= D3DX_PI * 2;
		}
		if (DifRot.x <= -D3DX_PI)
		{
			DifRot.x += D3DX_PI * 2;
		}
		if (DifRot.y <= -D3DX_PI)
		{
			DifRot.y += D3DX_PI * 2;
		}
		if (DifRot.z <= -D3DX_PI)
		{
			DifRot.z += D3DX_PI * 2;
		}

		DigitPos = DifPos * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultPos() + NowPos;
		DigitRot = DifRot * fRatioFrame + m_apModelParts[nCntParts]->GetDefaultRot() + NowRot;

		m_apModelParts[nCntParts]->SetPos(DigitPos);
		m_apModelParts[nCntParts]->SetRot(DigitRot);
	}

	++m_NowFrame;

	if (m_NowFrame >= m_aMotion[m_NextMotion].aKetSet[0].nFrame)
	{
		m_CurKey = 0;
		m_NowFrame = 0;
		m_CurMotion = m_NextMotion;
		m_bBlend = false;
		SetNextKey();
	} 

	return m_bBlend;
}

//==========================================================================================
//���[�V�������t�@�C������ǂݍ���
//==========================================================================================
void CPlayerX::MotionDataLoad()
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

	pFile = fopen("data\\TEXT\\motion_Player.txt","r");
	
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
			
			//���[�V�����̓ǂݍ��݊J�n
			if (!strcmp(LoadData, "MOTIONSET"))
			{
				while (1)
				{
					fscanf(pFile, "%s", LoadData);

					if (!strcmp(LoadData, "END_MOTIONSET"))//�ǂݍ��݂��I��
					{
						break;
					}
					//���[�v�̔��f
					else if (!strcmp(LoadData, "LOOP"))
					{
						fscanf(pFile, "%s", LoadData);
						fscanf(pFile, "%d", &m_aMotion[nMotionCnt].bLoop);
					}

					//�S�̂̃L�[���̓ǂݍ���
					else  if (!strcmp(LoadData, "NUM_KEY"))
					{
						fscanf(pFile, "%s", LoadData);
						fscanf(pFile, "%d", &m_aMotion[nMotionCnt].nKeyNum);
					}

					//�e�L�[��ǂݍ���
					if (!strcmp(LoadData, "KEYSET"))
					{
						while (1)
						{
							fscanf(pFile, "%s", LoadData); 

							if (LoadData[0] == '#')
							{//������΂�
								fgets(LoadData, 100, pFile);
								continue;
							}

							if (!strcmp(LoadData, "END_KEYSET"))
							{
								//�ǂݍ��݂��I��
								break;
							}

							//���݂̃L�[�̃t���[������ǂݍ���
							else if (!strcmp(LoadData, "FRAME"))
							{
								fscanf(pFile, "%s", LoadData);
								fscanf(pFile, "%d", &m_aMotion[nMotionCnt].aKetSet[nKeySet].nFrame);
							}

							//���݂̃L�[�̓ǂݍ���
							if (!strcmp(LoadData, "KEY"))
							{
								while (1)
								{
									fscanf(pFile, "%s", LoadData);

									if (!strcmp(LoadData, "END_KEY"))
									{
										// �ǂݍ��݂��I��
										break;
									}

									//�e�p�[�c�̃��[�V����pos�l
									else if (!strcmp(LoadData, "POS"))
									{
										fscanf(pFile, "%s", LoadData);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].pos.x);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].pos.y);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].pos.z);
									}

									//�e�p�[�c�̃��[�V����rot�l
									else if (!strcmp(LoadData, "ROT"))
									{
										fscanf(pFile, "%s", LoadData);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].rot.x);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].rot.y);
										fscanf(pFile, "%f", &m_aMotion[nMotionCnt].aKetSet[nKeySet].aKey[nKey].rot.z);
									}
								}
								//�L�[�ԍ���i�߂�
								++nKey;
							}
						}
						//�L�[�ԍ����������A�L�[�Z�b�g�ԍ���i�߂�
						nKey = 0;
						++nKeySet;
					}
				}
				//�L�[�Z�b�g�ԍ����������A���[�V�����ԍ���i�߂�
				nKeySet = 0;
				++nMotionCnt;
			}
		}
	}
	else
	{
		assert(pFile == nullptr);
	}
}

void CPlayerX::TransformSound()
{
	std::random_device rnd;				// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
	std::mt19937 mt(rnd());				//  �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
	std::uniform_int_distribution<> rand_num(0, 2);     // [0, 1] �͈͂̈�l����
	switch (rand_num(mt))
	{
	case 0:
		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_TRANSFORM1);
		break;
	case 1:
		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_TRANSFORM2);
		break;
	case 2:
		CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_TRANSFORM3);
		break;
	}
}


void CPlayerX::ReticleController()
{
	D3DXVECTOR2 ReticleMove = CManager::GetInstance()->GetJoypad()->GetJoyStickVecR();
	D3DXVec2Normalize(&ReticleMove, &ReticleMove);
	m_pReticle->AddPos({ ReticleMove.x * 5,ReticleMove.y * 5,0.0f });
}


D3DXVECTOR3 CPlayerX::CameraPosDigit()
{
	D3DXVECTOR3 CamPos = m_pos;

	return CamPos;
}


D3DXVECTOR3 CPlayerX::RifleMtxSet()
{
	D3DXMATRIX RifleMtx = m_apModelParts[19]->GetWorldMatrix();
	D3DXMATRIX UseMtx;

	D3DXVECTOR3 Addpos = { 40.0f,0.0f,-4.5f };
	D3DXVECTOR3 Addrot = { 0.0f,0.0f,0.0f };

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&UseMtx);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		Addrot.y,
		Addrot.x,
		Addrot.z);
	D3DXMatrixMultiply(&UseMtx,
		&UseMtx,
		&mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		Addpos.x,
		Addpos.y,
		Addpos.z);
	D3DXMatrixMultiply(&UseMtx,
		&UseMtx,
		&mtxTrans);

	//���[���h�s���e�̍s��Ɋ|����
	D3DXMatrixMultiply(&UseMtx,
		&UseMtx,
		&RifleMtx);

	D3DXVECTOR3 RiflePos = { UseMtx._41,UseMtx._42,UseMtx._43 };

	return RiflePos;
}

//==============================�@�@���C�ƃ��b�V���̏Փ˂������������蔻��p�@�@=============================================================================
//

bool CPlayerX::TestUseMeshCollision()
{	//=============================		�n�`���b�V������		==========================================================================
	// �n�`����
	BOOL  bIsHit = false;
	float fLandDistance;
	DWORD dwHitIndex = 0U;
	float fHitU;
	float fHitV;
	LPD3DXMESH pMesh = nullptr;
	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_MESHOBJECT) {
					CMeshGround* pTest = dynamic_cast<CMeshGround*>(pObj);
					if (pTest != nullptr) {
						pMesh = pTest->GetMesh();
						if (pTest != nullptr) {
							// �n�`����
							LPD3DXMESH pMesh = nullptr;

							pMesh = pTest->GetMesh();
							D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
							D3DXVECTOR3 objpos = m_pos - pTest->GetPos();
							D3DXIntersect(pMesh, &objpos, &dir, &bIsHit, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, nullptr, nullptr);

							// ----- �ڒn������ -----
							if (bIsHit)
							{
								if (m_bTransformed)
								{
									m_pos.y += fLandDistance - m_move.y + 45.0f;
								}
								else
								{
									m_pos.y += fLandDistance - m_move.y;
								}
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool CPlayerX::MeshObstacle()
{	//=============================		��Q�����b�V������		==========================================================================
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();

	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_OBSTACLE) {
					CMeshObstacle* pTest = dynamic_cast<CMeshObstacle*>(pObj);

					if (pTest != nullptr) {
	
						D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

						if (pCollision->MeshToIntersectCollision(pTest, m_pos, dir, 10 + m_move.z))
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

//
//========================================================================================================================================

//==========================================================================================
// �A�C�e���擾����
//==========================================================================================
void CPlayerX::GetItem()
{
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();

	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_ITEMS) {
					C3DItem* pTest = dynamic_cast<C3DItem*>(pObj);

					if (pTest != nullptr) {
						D3DXVECTOR3 dirM = D3DXVECTOR3(100.0f, 300.0f, 300.0f);

						if (pCollision->SphireCollosion(m_pos,pTest->GetPos(),dirM, dirM))
						{
							if (!pTest->GetItemUse())
							{
								CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_ITEM);
								CScene* pScene = CManager::GetInstance()->GetScene();
								if (pScene->GetSceneMode() == CScene::MODE_GAME)
								{
									CGame* pGame = dynamic_cast<CGame*>(pScene);
									pGame->GetScore()->AddScore(50);
								}
							}
							pTest->GotThisItem();
						}
					}
				}
			}
		}
	}
}

//==========================================================================================
// ���{�b�g�`�Ԃ̂݁A�߂��̓G�����b�N�I�����鏈��
//==========================================================================================
D3DXVECTOR3 CPlayerX::LockOnEnemy()
{
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();
	D3DXVECTOR3 dirM = D3DXVECTOR3(3000.0f, 0, 0);

	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_ENEMY) {
					CEnemyBase* pTest = dynamic_cast<CEnemyBase*>(pObj);

					if (pTest != nullptr) {

						if (pTest->GetPos().z - m_pos.z < 4000.0f &&
							pTest->GetPos().z - m_pos.z > 1000.0f)
						{
							if (pCollision->SphireCollosion(m_pos, pTest->GetPos(), dirM, dirM))
							{
								pTest->LockOned();
								return pTest->GetPos();
							}
						}
					}
				}
			}
		}
	}

	return m_pReticle->GetPos();
}

//==========================================================================================
//�e�̔��˂𐧌䂷�鏈��
//==========================================================================================
void CPlayerX::ShootBullet()
{
	D3DXVECTOR3 lockonVec;
	BossAttackCollision();
	if (m_bTransformed)
	{
		lockonVec = LockOnEnemy();
	}
	if (CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_LEFT_TRIGGER) == true && !m_bMotion && !m_bDamaged)
	{
		D3DXVECTOR3 digitRot = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 posMtx = { m_apModelParts[19]->GetWorldMatrix()._41, m_apModelParts[19]->GetWorldMatrix()._42 ,m_apModelParts[19]->GetWorldMatrix()._43 };

		digitRot.x = (m_pReticle->GetPos().x - posMtx.x) * 0.2f;
		digitRot.y = (m_pReticle->GetPos().y - posMtx.y) * 0.2f;
		digitRot.z = (m_pReticle->GetPos().z - posMtx.z) * 0.2f;
		D3DXVECTOR3 SetdigitedRot = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 addZpos = { 0.0f,0.0f,200.0f };
		D3DXVec3Normalize(&SetdigitedRot, &digitRot);
		SetdigitedRot.z;
		if (m_bTransformed)
		{
			if (m_CurMotion != MOTION_ROBO_SHOT)
			{
				SetNextMotion(MOTION_ROBO_SHOT);
				m_bMotion = true;
				digitRot.x = (lockonVec.x - posMtx.x) * 0.2f;
				digitRot.y = (lockonVec.y - posMtx.y) * 0.2f;
				digitRot.z = (lockonVec.z - posMtx.z) * 0.2f;
				D3DXVECTOR3 SetdigitedRot = { 0.0f,0.0f,0.0f };
				D3DXVec3Normalize(&SetdigitedRot, &digitRot);
				if (lockonVec != m_pReticle->GetPos())
				{
					CBullet3D::Create(RifleMtxSet() + m_move + addZpos, SetdigitedRot, { 0.1f,1.0f,0.2f,1.0f }, 80, 38, 35);
				}
				else
				{
					CBullet3D::Create(RifleMtxSet() + m_move + addZpos, SetdigitedRot, { 1.0f,0.0f,0.2f,1.0f }, 80, 38, 35);
				}
			}
		}
		else
		{
			CBullet3D::Create(RifleMtxSet() + m_move + addZpos, SetdigitedRot, { 1.0f,0.0f,0.0f,1.0f }, 80, 32, 22);
		}
	}
	if (CManager::GetInstance()->GetJoypad()->GetTrigger(CJoypad::JOYPAD_RIGHT_TRIGGER) == true &&
		!m_bMotion && !m_bDamaged && m_bTransformed&& 
		m_CurMotion != MOTION_ROBO_SLASH)
	{
		SetNextMotion(MOTION_ROBO_SLASH);
		m_bAttack = true;
	}
	if (lockonVec != m_pReticle->GetPos() && m_bTransformed)
	{
		m_vecAxis = { 0.0f,1.0f,0.0f };
		D3DXVec3Normalize(&m_vecAxis, &m_vecAxis);

		m_fValueRot = atan2f((lockonVec.x - m_pos.x), (lockonVec.z - m_pos.z));
		if (m_CurMotion == MOTION_ROBO_SLASH && m_bAttack&& CheckLockonBlocking())
		{
			AttackCollisionToEnemy();
			m_move += ((lockonVec - m_pos) * 0.015);
		}
	}
}

//==========================================================================================
// ���b�N�I�������G�����j���鏈��
//==========================================================================================
void CPlayerX::AttackCollisionToEnemy()
{
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();
	
	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_ENEMY) {
					CEnemyBase* pTest = dynamic_cast<CEnemyBase*>(pObj);
					D3DXVECTOR3 SetRadius = { 700.0f,0.0f,0.0f };
					if (pTest != nullptr) {
						if (pCollision->SphireCollosion(m_pos, pTest->GetPos(), SetRadius, SetRadius))
						{
							m_bAttack = false;
							pTest->Damaged();
						}
					}
				}
			}
		}
	}
}

//==========================================================================================
// �{�X�̍U���Ƃ̔��菈��
//==========================================================================================
void CPlayerX::BossAttackCollision()
{
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();

	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_BOSSBILLET) {
					CBossBullet *pTest = dynamic_cast<CBossBullet*>(pObj);
					D3DXVECTOR3 SetRadius = { 50.0f,0.0f,0.0f };
					if (pTest != nullptr) {
						if (pCollision->SphireCollosion(m_pos, pTest->GetPos(), SetRadius, SetRadius))
						{
							SetDamageState();
							return;
						}
					}
				}
				if (type == CObject::TYPE::TYPE_3D_BOSSWEAPONS) {
					CBossKnife* pTest = dynamic_cast<CBossKnife*>(pObj);
					D3DXVECTOR3 SetRadius = { 70.0f,0.0f,0.0f };
					if (pTest != nullptr) {
						if (pCollision->SphireCollosion(m_pos, pTest->GetPos(), SetRadius, SetRadius))
						{
							if (m_CurMotion == MOTION_ROBO_SLASH)
							{
								pTest->Braking();
							}
							else
							{
								SetDamageState();
							}
							return;
						}
					}
				}
			}
		}
	}
}

//==========================================================================================
// ���b�N�I����̓G�Ƃ̊Ԃɏ�Q�����Ȃ������ׂ鏈��
//==========================================================================================
bool CPlayerX::CheckLockonBlocking()
{
	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_ENEMY) {
					CEnemyBase* pTest = dynamic_cast<CEnemyBase*>(pObj);

					if (pTest != nullptr) {
						bool bCheck = CheckToObs(pTest);
						return bCheck;
					}
				}
			}
		}
	}

	return false;
}

bool CPlayerX::CheckToObs(CEnemyBase* pEnemy)
{
	std::unique_ptr<CCollision> pCollision = std::make_unique<CCollision>();

	for (int j = 0; j < SET_PRIORITY; ++j) {
		for (int i = 0; i < MAX_OBJECT; ++i) {
			CObject* pObj = CObject::GetObjects(j, i);
			if (pObj != nullptr) {
				CObject::TYPE type = pObj->GetType();
				if (type == CObject::TYPE::TYPE_3D_OBSTACLE) {
					CMeshObstacle* pTest = dynamic_cast<CMeshObstacle*>(pObj);

					if (pTest != nullptr) {

						D3DXVECTOR3 toVec = pEnemy->GetPos() - m_pos;
						D3DXVECTOR3 dir;
						float toLength = D3DXVec3Length(&toVec);
						D3DXVec3Normalize(&dir, &toVec);
						if (pCollision->MeshToIntersectCollision(pTest, m_pos, dir, toLength))
						{
							return false;
						}
					}
				}
			}
		}
	}
	return true;
}

//==========================================================================================
// �_���[�W���Z�A�����m�F�A�q�b�g���Ȃ炵�Ĕ����G�t�F�N�g����
//==========================================================================================
void CPlayerX::DamageAdd(int nDmg)
{ 
	m_nLife -= nDmg;
	DeadCheck(); 
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_GAMESE_EXPLOAD); 
	CEffBomb::Create(m_pos); 
}

//==========================================================================================
// �ړ��Œ胂�[�h�m�F
//==========================================================================================
bool CPlayerX::PushRSholder()
{
	if (!m_bTransformed)
	{
		if (CManager::GetInstance()->GetJoypad()->GetRepeat(CJoypad::JOYPAD_RIGHT_TRIGGER) == true)
		{
			return true;
		}
	}
	return false;
}