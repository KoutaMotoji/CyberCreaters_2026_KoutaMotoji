//===============================================================================
//
//  C++使った2D(particle.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "object2D.h"


class CParticle :public CObject2D
{
public:
	CParticle();					//コンストラクタ
	~CParticle()override;			//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static void Load();
	static void Unload();
	static CParticle* Create(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9 m_pTextureTemp;
	static const float PARTICLE_HEIGHT;
	static const float PARTICLE_WIDTH;
};



#endif