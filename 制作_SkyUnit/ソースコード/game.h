//===============================================================================
//
//  ゲームシーン(game.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "scene.h"
#include "score.h"
#include "mesh_obstacle.h"

constexpr int MAX_TREESET_NUM = 260;

class CGame : public CScene
{
public:
	CGame();
	~CGame()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	inline CScore* GetScore() { return m_Score; }
private:
	void MakeRandTree();

	CScore* m_Score;
};

#endif