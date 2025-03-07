//===============================================================================
//
//  �e�N�X�`���ꊇ�Ǘ��N���X(texture.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#define MAX_TEXTURE	(64)

class CTexture
{
public:
	CTexture();
	~CTexture();
	void Unload();
	int Regist(const char* pTex);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
private:
	LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];
	const char* m_aTexName[MAX_TEXTURE];
	int m_nNumAll;
};

#endif