#include "stdafx.h"
#include "Warp.h"

bool Warp::Start()
{
	//�S�[�X�g�I�u�W�F�N�g�̏������B
	m_physicsGhostObj.CreateBox(Vector3(100.0f, 100.0f, 200.0f),
		Quaternion::Identity, Vector3::One * 140.0f);

	return true;
}