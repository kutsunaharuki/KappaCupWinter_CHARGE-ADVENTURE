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
	//注視点からのベクトルを設定する。
	m_toCameraPos.Set(200.0f, 180.0f, -420.0f);
	m_springCamera.Init(
		*g_camera3D,//バネカメラの処理を行うカメラを指定する。
		1000.0f,	//カメラの移動速度の最大値。
		true,		//カメラと地形とのあたり判定をとるかどうかのフラグ。trueだと当たり判定を行う。
		5.0f		//カメラに認定された球体コリジョンの半径。第3引数がtrueの時に有効になる。
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

	//Y軸周りの回転。
	qrot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qrot.Apply(m_toCameraPos);

	//X軸周りの回転。
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