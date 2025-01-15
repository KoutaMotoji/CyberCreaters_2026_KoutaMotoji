//===============================================================================
//
//  3D�Q�[�����(fog.h)
//								����F���n�O��
// 
//===============================================================================
#include "fog.h"
#include "manager.h"

CFog::CFog()
{
}

CFog::~CFog()
{
}
/// <summary>
/// �͈͎w��Ńt�H�O���쐬
/// </summary>
void CFog::SetFogLinear(float Start, float End)
{
	float fStart, fEnd;
	fStart = Start;
	fEnd = End;

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�t�H�O�̐ݒ�
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);		//�͈͎w��ɐݒ�
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));

	//�͈͎w��̐ݒ�
	//m_fFogStartPos = 4500.0f;
	//m_fFogEndPos = 15000.0f;
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStart));
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEnd));
}

/// <summary>
/// ���x�w��Ńt�H�O���쐬
/// </summary>
void CFog::SetFogDensity(float Density)
{
	float fDentisy;
	fDentisy = Density;

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�t�H�O�̐ݒ�
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DRS_FOGDENSITY);		//�͈͎w��ɐݒ�
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));

	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));

}

/// <summary>
/// �t�H�O���I��
/// </summary>
void CFog::FinishFog()
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}