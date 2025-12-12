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

	SpringCamera m_springCamera;//バネカメラ。
	Vector3 m_toCameraPos = Vector3::Zero;
private:
	Player* m_player = nullptr;
};

