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

	Vector3 m_toCameraPos = Vector3::Zero;
	
	Player* m_player = nullptr;
	SpringCamera m_springCamera;//バネカメラ。
};

