//===============================================================================
//
//  C++�g����3D(result.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "scene.h"
#include "object2D.h"
#include "score.h"

class CGameover : public CScene
{
public:
	CGameover();
	~CGameover()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:
	CScore* m_pScore;
};

class CGameoverBG :public CObject2D
{
public:
	CGameoverBG() {};		//�R���X�g���N�^
	~CGameoverBG()override = default;				//�f�X�g���N�^
	void Init()override;		//������

	static CGameoverBG* Create();
private:
};

#endif