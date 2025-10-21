#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_player = FindGO<Player>("player");
	//�����_����̃x�N�g����ݒ肷��B
	m_toCameraPos.Set(200.0f, 180.0f, -420.0f);
	m_springCamera.Init(
		*g_camera3D,//�o�l�J�����̏������s���J�������w�肷��B
		1000.0f,	//�J�����̈ړ����x�̍ő�l�B
		true,		//�J�����ƒn�`�Ƃ̂����蔻����Ƃ邩�ǂ����̃t���O�Btrue���Ɠ����蔻����s���B
		5.0f		//�J�����ɔF�肳�ꂽ���̃R���W�����̔��a�B��3������true�̎��ɗL���ɂȂ�B
	);

	g_camera3D->SetNear(10.0f);
	g_camera3D->SetFar(100000.0f);
	return true;
}

void GameCamera::Update()
{
	Vector3 target = m_player->m_position;
	target.y += 80.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	Vector3 m_toCameraPodOld = m_toCameraPos;

	//Y������̉�]�B
	qrot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qrot.Apply(m_toCameraPos);

	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qrot.SetRotationDeg(axisX, 2.0f * y);
	qrot.Apply(m_toCameraPos);

	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.7f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f)
	{
		m_toCameraPos = toCameraPosOld;
	}

	Vector3 pos = target + m_toCameraPos;
	m_springCamera.SetTarget(target);
	m_springCamera.SetPosition(pos);

	m_springCamera.Update();
}