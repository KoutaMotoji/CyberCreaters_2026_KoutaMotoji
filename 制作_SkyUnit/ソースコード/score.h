//===============================================================================
//
//		スコア管理クラス(score.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"
#include "number.h"

static constexpr int MAX_DIGIT = 8;
static constexpr float MAX_HEIGHT = 50.0f;
static constexpr float MAX_WIDTH = 80.0f;

class CScore
{
public:
	CScore();		//コンストラクタ
	~CScore();		//デストラクタ
	void Init();		//初期化
	void Init(D3DXVECTOR3 pos);		//初期化

	void Uninit();		//終了
	void Update();		//更新
	void Draw();		//描画
	static CScore* Create();
	static CScore* Create(D3DXVECTOR3 pos);

	inline void LoadLastScore() { m_Score = SaveScore; GetLastNum(); }
	inline void ResetScore() { m_Score += 0; GetLastNum(); SaveScore = m_Score;};
	inline void SetScore(int nScore) { m_Score = nScore; GetLastNum(); SaveScore = m_Score; }
	inline void AddScore(int nScore) { m_Score += nScore; GetLastNum(); SaveScore = m_Score;}
private:
	void GetLastNum();
	int m_Score;

	static const D3DXVECTOR3 DEFAULT_POSITION;
	static int SaveScore;
	CNumber* m_number[MAX_DIGIT];
};

#endif