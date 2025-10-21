#include "stdafx.h"
#include "Warp.h"

bool Warp::Start()
{
	//ゴーストオブジェクトの初期化。
	m_physicsGhostObj.CreateBox(Vector3(100.0f, 100.0f, 200.0f),
		Quaternion::Identity, Vector3::One * 140.0f);

	return true;
}