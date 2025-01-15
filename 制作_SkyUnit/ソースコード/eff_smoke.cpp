//===============================================================================
//
//  C++�g����2D(eff_smoke.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "eff_smoke.h"
#include "manager.h"

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CEffSmoke::CEffSmoke(int nPriority) :CBillboard(nPriority), m_nTime(0)
{
	int nIdx = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\smoke.png");
	BindTexture(CManager::GetInstance()->GetTexture()->GetAddress(nIdx), { 1,1 });
}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CEffSmoke::~CEffSmoke()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CEffSmoke::Init()
{
	CObject::SetType(TYPE_BILLBOARD);
	m_moveValue = SetRandValue();
	CBillboard::Init();
}

//==========================================================================================
//�I������
//==========================================================================================
void CEffSmoke::Uninit()
{
	CBillboard::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CEffSmoke::Update()
{
	--m_nLife;
	if (m_nLife < 0)
	{
		CObject::Release();
		return;
	}
	CBillboard::AddPos(m_moveValue);
	CBillboard::AddRot({ 0.0f,0.0f,m_fRotValue });
	CBillboard::AddScale(0.05f);

	CBillboard::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CEffSmoke::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	// �@���̎������K����L����
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//�J�����O�𗼖ʂ�

	CBillboard::Draw();

	// �@���̎������K���𖳌���
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	//�J�����O�����ǂ���

}

//==========================================================================================
//��������
//==========================================================================================
CEffSmoke* CEffSmoke::Create(D3DXVECTOR3 pos, float Radius,int nLife)
{
	CEffSmoke* Effect = new CEffSmoke;

	Effect->SetPolygonParam(pos, Radius, Radius);
	Effect->Init();
	Effect->m_nLife = nLife;
	Effect->m_nTime = 0;
	return Effect;
}

//==========================================================================================
//��������
//==========================================================================================
D3DXVECTOR3 CEffSmoke::SetRandValue()
{
	D3DXVECTOR3 p;
	CEffSmoke* Effect = new CEffSmoke;

	std::random_device rnd;			// �񌈒�I�ȗ���������ŃV�[�h�����@�𐶐�
	std::mt19937 mt(rnd());			//  �����Z���k�c�C�X�^�[��32�r�b�g�ŁA�����͏����V�[�h
	std::uniform_int_distribution<> rand_x(-8, 8);	// [-3000, 3000] �͈͂̈�l����
	std::uniform_int_distribution<> rand_y(10, 30);		// [2000, 5000] �͈͂̈�l����
	std::uniform_int_distribution<> rand_z(-8, 8);	// [2000, 5000] �͈͂̈�l����
	std::uniform_int_distribution<> rotV(800, 1500);	// [2000, 5000] �͈͂̈�l����
	std::uniform_int_distribution<> addSize(80, 150);	// [2000, 5000] �͈͂̈�l����

	p = {
		(float)(rand_x(mt))* 0.15f,
		(float)(rand_y(mt))* 0.15f,
		(float)(rand_z(mt))* 0.15f
	};

	m_fRotValue = ((float)rotV(mt) * 0.0001);
	AddScale((float)(addSize(mt) * 0.008f));
	return p;
}