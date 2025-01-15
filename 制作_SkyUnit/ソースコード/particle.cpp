//===============================================================================
//
//  C++�g����2D(bg.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "particle.h"
#include "manager.h"

//�ÓI�����o������
const float CParticle::PARTICLE_HEIGHT = 50.0f;
const float CParticle::PARTICLE_WIDTH = 50.0f;
LPDIRECT3DTEXTURE9 CParticle::m_pTextureTemp = nullptr;

//==========================================================================================
//�R���X�g���N�^
//==========================================================================================
CParticle::CParticle()
{

}

//==========================================================================================
//�f�X�g���N�^
//==========================================================================================
CParticle::~CParticle()
{

}

//==========================================================================================
//����������
//==========================================================================================
void CParticle::Init()
{
	CObject::SetType(TYPE_BG);
	CObject2D::Init();
}

//==========================================================================================
//�I������
//==========================================================================================
void CParticle::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================================
//�X�V����
//==========================================================================================
void CParticle::Update()
{
	CObject2D::SetZoom(-0.5f);
	if (CObject2D::GetSize() <= 10)
	{
		CObject::Release();
		return;
	}

	CObject2D::Update();
}

//==========================================================================================
//�`�揈��
//==========================================================================================
void CParticle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();;

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//�ʏ�̍����ɖ߂��ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================================================
//��������
//==========================================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos)
{
	CParticle* particle = new CParticle;

	particle->BindTexture(m_pTextureTemp, 1, 1);
	particle->SetPolygonParam(pos, PARTICLE_HEIGHT, PARTICLE_WIDTH, {0.0f,0.5f,0.2f,1.0f});
	particle->Init();
	return particle;
}

//==========================================================================================
//�e�N�X�`������
//==========================================================================================
void CParticle::Load()
{
	if (m_pTextureTemp == nullptr)
	{
		LPDIRECT3DDEVICE9 pDevice;
		//�f�o�C�X�̎擾
		pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\shadow000.jpg",
			&m_pTextureTemp);
	}
}

//==========================================================================================
//�e�N�X�`���j��
//==========================================================================================
void CParticle::Unload()
{
	//�e�N�X�`���̔j��
	if (m_pTextureTemp != nullptr)
	{
		m_pTextureTemp->Release();
		m_pTextureTemp = nullptr;
	}
}