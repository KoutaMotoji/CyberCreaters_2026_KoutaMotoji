//===============================================================================
//
//  テクスチャ一括管理クラス(texture.cpp)
//								制作：元地弘汰
// 
//===============================================================================

#include "main.h"
#include "texture.h"
#include "manager.h"

CTexture::CTexture():m_nNumAll(0)
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		m_pTexture[i] = nullptr;
		m_aTexName[i] = nullptr;
	}
}

CTexture::~CTexture()
{

}

void CTexture::Unload()
{
	for (int i = 0; i < m_nNumAll; i++)
	{
		if (m_pTexture[i] != nullptr)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = nullptr;
			m_aTexName[i] = nullptr;
		}
	}
}

int CTexture::Regist(const char* pTex)
{
	int nIdx = 0;
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (m_pTexture[i] == nullptr)
		{
			LPDIRECT3DDEVICE9 pDevice;
			//デバイスの取得
			pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
			D3DXCreateTextureFromFile(pDevice,
				pTex,
				&m_pTexture[i]);
			nIdx = i;
			m_aTexName[i] = pTex;
			m_nNumAll++;
			break;
		}
		else if (pTex == m_aTexName[i])
		{
			nIdx = i;
			break;
		}
	}
	return nIdx;
}

LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return m_pTexture[nIdx];
}