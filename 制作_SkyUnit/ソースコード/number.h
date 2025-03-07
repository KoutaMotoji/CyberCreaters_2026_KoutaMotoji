//===============================================================================
//
//  スコア表示の数字ポリゴン表示(number.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "object2D.h"


class CNumber :public CObject2D
{
public:
	CNumber(int nPriority = 6);		//コンストラクタ
	~CNumber()override;				//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	static CNumber* Create(D3DXVECTOR3 pos,float Height,float Width);

	void SetNum(int Num) { 
		m_nNum = Num;	
		CObject2D::SetAnim({ (float)m_nNum,CObject2D::GetAnim().y });
	};							//自身の桁の数字を取得、アニメーションを移動
private:
	int m_nNum;
};

#endif