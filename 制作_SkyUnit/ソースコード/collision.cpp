//===============================================================================
//
//  C++使った2D(start.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "collision.h"

//==========================================================================================
//コンストラクタ
//==========================================================================================
CCollision::CCollision()
{

}

//==========================================================================================
//デストラクタ
//==========================================================================================
CCollision::~CCollision()
{

}

//==========================================================================================
//円形当たり判定
//==========================================================================================
bool CCollision::CircleCollosion(D3DXVECTOR3 MainPos, D3DXVECTOR3 SubPos, D3DXVECTOR3 MainRadius, D3DXVECTOR3 SubRadious)
{
	float fLengthX = MainPos.x - SubPos.x;
	float fLengthY = MainPos.y - SubPos.y;

	float CenterDistance = sqrtf((fLengthX * fLengthX) + (fLengthY * fLengthY));
	float fAngle = ((MainRadius.x + SubRadious.x) + (MainRadius.y + MainRadius.y)) * 0.5f;

	if (CenterDistance <= fAngle)
	{
		return true;
	}

	return false;
}

//==========================================================================================
//球形当たり判定
//==========================================================================================
bool CCollision::SphireCollosion(D3DXVECTOR3 MainPos, D3DXVECTOR3 SubPos, D3DXVECTOR3 MainRadius, D3DXVECTOR3 SubRadious)
{
	float fLengthX = MainPos.x - SubPos.x;
	float fLengthY = MainPos.y - SubPos.y;
	float fLengthZ = MainPos.z - SubPos.z;
	float fRadiusM, fRadiusS;
	fRadiusM = MainRadius.x;
	fRadiusS = SubRadious.x;


	if ((fLengthX * fLengthX) + (fLengthY * fLengthY) + (fLengthZ * fLengthZ) <= ((fRadiusM + fRadiusS) * (fRadiusM + fRadiusS)))
	{
		return true;
	}

	return false;
}

//==========================================================================================
//飛ばしたレイとメッシュの判定
//==========================================================================================
bool CCollision::MeshToIntersectCollision(LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance)
{
	// 地形判定
	BOOL  bIsHit = false;
	float fLandDistance;
	DWORD dwHitIndex = 0U;
	float fHitU;
	float fHitV;

	D3DXIntersect(pMesh, &pos, &dir, &bIsHit, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, nullptr, nullptr);

	return (bIsHit && fLandDistance <= CheckDistance);
}

//==========================================================================================
//飛ばしたレイとメッシュの判定(移動・回転等した時の処理を含むオーバーロード)
//==========================================================================================
bool CCollision::MeshToIntersectCollision(CMeshObstacle* pObs,D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance)
{
	// 地形判定
	BOOL  bIsHit = false;
	float fLandDistance;
	DWORD dwHitIndex = 0U;
	float fHitU;
	float fHitV;

	LPD3DXMESH pMesh = nullptr;
	D3DXMATRIX mWorld;
	D3DXVECTOR3 vStartl;
	D3DXVECTOR3 vDirl;
	D3DXVECTOR3 vEnd;

	pMesh = pObs->GetMesh();
	vEnd = pos + dir;
	D3DXMATRIX objMtx = pObs->GetMatrix();

	// レイを当てる対象のマトリックスの逆行列を取得し、始点と終点の座標に対して座標変換を行い、位置・回転・大きさの補間をする
	D3DXMatrixInverse(&mWorld, NULL, &objMtx);
	D3DXVec3TransformCoord(&vStartl, &pos, &mWorld);
	D3DXVec3TransformCoord(&vEnd, &vEnd, &mWorld);

	vDirl = vEnd - vStartl;

	D3DXIntersect(pMesh, &vStartl, &vDirl, &bIsHit, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, nullptr, nullptr);

	return (bIsHit && fLandDistance <= CheckDistance);
}

//==========================================================================================
//飛ばしたレイとメッシュの判定(交差距離を渡すオーバーロード)
//==========================================================================================
bool CCollision::MeshToIntersectCollision(float* Distance,LPD3DXMESH pMesh, D3DXVECTOR3 pos, D3DXVECTOR3 dir, float CheckDistance)
{
	// 地形判定
	BOOL  bIsHit = false;
	float fLandDistance;
	DWORD dwHitIndex = 0U;
	float fHitU;
	float fHitV;

	D3DXIntersect(pMesh, &pos, &dir, &bIsHit, &dwHitIndex, &fHitU, &fHitV, &fLandDistance, nullptr, nullptr);
	*Distance = fLandDistance;
	return (bIsHit && fLandDistance <= CheckDistance);
}