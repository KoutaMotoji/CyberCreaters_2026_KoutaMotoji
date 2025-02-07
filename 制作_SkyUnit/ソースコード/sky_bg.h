//===============================================================================
//
//  C++使った3D(field.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _SKY_BG_H_
#define _SKY_BG_H_

#include "main.h"
#include "object3D.h"


class CSkyBg :public CObject3D
{
public:
	CSkyBg();					//コンストラクタ
	~CSkyBg()override;			//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	static CSkyBg* Create(D3DXVECTOR3 pos);

private:
	static constexpr float FLOOR_SIZE = 15000.0f;
};



#endif