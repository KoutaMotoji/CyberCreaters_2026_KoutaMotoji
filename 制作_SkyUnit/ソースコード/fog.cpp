//===============================================================================
//
//  3Dゲーム基盤(fog.h)
//								制作：元地弘汰
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
/// 範囲指定でフォグを作成
/// </summary>
void CFog::SetFogLinear(float Start, float End)
{
	float fStart, fEnd;
	fStart = Start;
	fEnd = End;

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//フォグの設定
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);		//範囲指定に設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));

	//範囲指定の設定
	//m_fFogStartPos = 4500.0f;
	//m_fFogEndPos = 15000.0f;
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStart));
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEnd));
}

/// <summary>
/// 密度指定でフォグを作成
/// </summary>
void CFog::SetFogDensity(float Density)
{
	float fDentisy;
	fDentisy = Density;

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//フォグの設定
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DRS_FOGDENSITY);		//範囲指定に設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));

	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));

}

/// <summary>
/// フォグを終了
/// </summary>
void CFog::FinishFog()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}