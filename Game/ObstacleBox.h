#pragma once
class Player;
class ObstacleBox : public IGameObject
{
public:
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);
	void SetCollisionObj();
private:
	ModelRender m_boxRender;
	Vector3 m_boxPos = { -150.0f,0.0f,-120.0f };//ボックスの座標。
	Vector3 m_boxSc = { 0.2f,0.2f,0.2f };       //ボックスの大きさ。
	Vector3 m_collisionSc = { 1.2f,0.9f,1.2f }; //コリジョンの大きさ。
	CollisionObject* m_collisionObj;            //コリジョンオブジェクト。
	PhysicsStaticObject m_physicsStaticObjBox;  //静的オブジェクト。
	Quaternion m_brot = Quaternion::Identity;   //回転。
	Player* m_player = nullptr;
};

