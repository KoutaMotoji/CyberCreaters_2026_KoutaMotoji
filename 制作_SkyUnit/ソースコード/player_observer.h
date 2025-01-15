//===============================================================================
//
//  プレイヤー監視クラス(player_observer.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _PLAYER_OBSERVER_H_
#define _PLAYER_OBSERVER_H_

#include "main.h"
#include "playerX.h"

//プレイヤー監視クラス
class CPlayerObserver
{
private:
	CPlayerX* pPlayer;
	static std::shared_ptr<CPlayerObserver> _instance;
public:
	CPlayerObserver() { pPlayer = nullptr; }
	//シングルトン用内部インスタンス
	static const std::shared_ptr<CPlayerObserver> GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = std::make_shared<CPlayerObserver>();
		}
		return _instance;
	}
	void PlayerSearch();
	void PlayerObsDestroy();
	D3DXVECTOR3 GetPlayerPos();
	CPlayerX* GetPlayer();
	D3DXVECTOR3 GetPlayerMove();
	int GetPlayerLife();
};

#endif 
