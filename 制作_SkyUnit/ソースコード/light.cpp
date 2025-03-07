//===============================================================================
//
//		ライト処理(light.h)
//								制作：元地弘汰
// 
//===============================================================================
#include "light.h"
#include "manager.h"

CLight::CLight()
{
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&m_aLight[i], sizeof(D3DLIGHT9));
	}
}

CLight::~CLight()
{
}
//初期化
HRESULT CLight::Init(void)
{
	
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir[3];

	for (int i = 0; i < MAX_LIGHT; i++)
	{
		//ライトの種類を設定
		m_aLight[i].Type = D3DLIGHT_DIRECTIONAL;
		//ライトの拡散光
		m_aLight[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//ライトの方向を設定
		switch (i)
		{
		case 0:
			vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
			break;
		case 1:
			vecDir[1] = D3DXVECTOR3(-0.5f, -0.3f, 0.2f);
			break;
		case 2:
			vecDir[2] = D3DXVECTOR3(0.6f, -0.6f, -0.2f);
			break;
		}
		D3DXVec3Normalize(&vecDir[i], &vecDir[i]);
		m_aLight[i].Direction = vecDir[i];

		//ライトを設定する
		pDevice->SetLight(i, &m_aLight[i]);
		//ライトを有効にする
		pDevice->LightEnable(i, TRUE);
	}

	return S_OK;
}

//終了
void CLight::Uninit(void)
{

}

//更新
void CLight::Update(void)
{

}