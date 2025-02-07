//===============================================================================
//
//  C++�g����3D(result.h)
//								����F���n�O��
// 
//===============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "scene.h"
#include "object2D.h"
#include "score.h"
#include "go_UI.h"

class CResult : public CScene
{
public:
	CResult();
	~CResult()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
private:
	CScore* m_pScore;
	CGameOverUI* m_pGOUI;
	bool m_bSelect;
};

class CResultBG :public CObject2D
{
public:
	CResultBG() {};		//�R���X�g���N�^
	~CResultBG()override = default;				//�f�X�g���N�^
	void Init()override;		//������

	static CResultBG* Create();
private:
};

#endif