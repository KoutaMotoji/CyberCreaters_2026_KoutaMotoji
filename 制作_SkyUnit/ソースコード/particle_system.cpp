//===============================================================================
//
//  C++�g����3D(particle_system.cpp)
//								����F���n�O��
// 
//===============================================================================

#include "particle3D.h"
#include "particle_system.h"


//==========================================================================================================================
// �p�����ăG�t�F�N�g���o���N���X�Q

/// <summary>
/// �����ˏo���@���p���̏ꍇ�̃C���X�^���X��n������
/// </summary>
CEmitte_Loop* CEmitte_Loop::Instance(CEmitte_Form* Beh, int nLoopTime, int MaxNum)
{
	CEmitte_Loop* pLoop = new CEmitte_Loop();

	pLoop->m_LoopTime = nLoopTime;
	pLoop->m_nRestNum = MaxNum;
	pLoop->m_pForm = Beh;
	return pLoop;
}

//���Ԋu�Ńp�[�e�B�N������o����
void CEmitte_Loop::SetEmitte()
{
	if (m_nTimer == m_LoopTime)
	{
		m_nTimer = 0;
		m_pForm->DoEmitte();
		CEmitte_Type::SetEmitte();
	}
	else
	{
		++m_nTimer;
	}
}

//==========================================================================================================================
// �����I�ɃG�t�F�N�g���o���N���X�Q

/// <summary>
/// �����ˏo���@�������̏ꍇ�̃C���X�^���X��n������
/// </summary>
CEmitte_Bomb* CEmitte_Bomb::Instance(CEmitte_Form* Beh, int MaxNum)
{
	CEmitte_Bomb* pBomb = new CEmitte_Bomb();

	pBomb->m_nRestNum = MaxNum;
	pBomb->m_pForm = Beh;
	return pBomb;
}

//��C�Ƀp�[�e�B�N������o����
void CEmitte_Bomb::SetEmitte()
{
	int MAX = m_nRestNum;
	for (int i = 0; i < MAX; ++i)
	{	//�w��̌�����C�Ƀp�[�e�B�N���𐶐�
		m_pForm->DoEmitte();
		CEmitte_Type::SetEmitte();
	}
}

//==========================================================================================================================

//==========================================================================================================================
//�p�[�e�B�N���̔��ˌ`����Ǘ�����N���X�Q�̏���

//************************************************
//�e�`��N���X�̃C���X�^���X
CEmitte_Line* CEmitte_Line::Instance(D3DXVECTOR3 dir, int MaxValue, int MinValue)
{
	CEmitte_Line* pLine = new CEmitte_Line();
	pLine->m_direction = dir;
	pLine->m_MaxValue = MaxValue;
	pLine->m_MinValue = MinValue;
	return pLine;
}

CEmitte_Sphere* CEmitte_Sphere::Instance(int MaxValue, int MinValue)
{
	CEmitte_Sphere* pSphere = new CEmitte_Sphere();
	pSphere->m_MaxValue = MaxValue;
	pSphere->m_MinValue = MinValue;
	return pSphere;
}

CEmitte_Cone* CEmitte_Cone::Instance(D3DXVECTOR3 dir,float Radius, int MaxValue, int MinValue)
{
	CEmitte_Cone* pCone = new CEmitte_Cone();
	pCone->m_direction = dir;
	pCone->m_fRadius = Radius;
	pCone->m_MaxValue = MaxValue;
	pCone->m_MinValue = MinValue;
	return pCone;
}
//************************************************

void CEmitte_Line::DoEmitte()
{
	//��������
	std::random_device rnd;				// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
	std::mt19937 mt(rnd());				//  �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
	std::uniform_int_distribution<> rand_x(m_MinValue, m_MaxValue);     // [-1200, 1200] �͈͂̈�l����
	
}

//==========================================================================================================================
