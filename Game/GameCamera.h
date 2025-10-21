#pragma once
#include "camera/SpringCamera.h"
class Player;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start()override;
	void Update();

	Quaternion qrot;
	Vector3 m_toCameraPos;
	Vector3 CameraPos;
	Vector3 targetPos;
	
	Player* m_player = nullptr;
	SpringCamera m_springCamera;//バネカメラ。
};

