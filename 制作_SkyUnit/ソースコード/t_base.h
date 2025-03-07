//===============================================================================
//
//  タイトルシーン用建物オブジェクト処理(t_base.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _T_BASE_H_
#define _T_BASE_H_

#include "main.h"
#include "objectX.h"


class CTitleBase :public CObjectX
{
public:

	CTitleBase();				//コンストラクタ
	~CTitleBase()override;		//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画
	static CTitleBase* Create(D3DXVECTOR3 pos);
protected:

private:

};



#endif