//===============================================================================
//
//  C++使った3D(t_anim_border.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _TITLEANIM_BORDER_H_
#define _TITLEANIM_BORDER_H_

#include "main.h"
#include "object2D.h"


class CTAnimBorder :public CObject2D
{
public:
	CTAnimBorder(int nPriority = SET_PRIORITY - 2);		//コンストラクタ
	~CTAnimBorder()override;				//デストラクタ
	void Init()override;		//初期化
	void Uninit()override;		//終了
	void Update()override;		//更新
	void Draw()override;		//描画

	static CTAnimBorder* Create(int type);

protected:
	virtual void SetPosToAnim() {};
private:
};

class CTAnimBorderU :public CTAnimBorder
{
public:
	void Init()override;		//初期化
	//描画
protected:
	void SetPosToAnim()override;
};

class CTAnimBorderD :public CTAnimBorder
{
public:
	void Init()override;		//初期化
	//描画
protected:
	void SetPosToAnim()override;
};
#endif