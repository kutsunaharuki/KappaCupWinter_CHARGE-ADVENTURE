#pragma once
class Player;
class MovingFloorUpDown : public IGameObject 
{
public:
	~MovingFloorUpDown();
	bool Start()override;
	void Update();
	void Move();
	void SetModel();
	void SetPhysicsStaticObj();
	void SetCollisionObj();
	void Render(RenderContext& rc);

	Vector3 m_firstPos = Vector3::Zero;                 //初期位置。
	Vector3 m_movingPos   = Vector3::Zero;              //現在地。
	Vector3 m_movingSpeed = Vector3::Zero;              //移動速度。
	Vector3 m_movingScale = Vector3::One;               //大きさ。
	
	Quaternion m_movingRot = Quaternion::Identity;      //回転。
private:
	ModelRender m_movingFloorUpDownRender;
	PhysicsStaticObject m_physicsStaticObj;             //静的物理オブジェクト。
	CollisionObject* m_collisionObj = nullptr;          //コリジョンオブジェクト。
	Player* m_player = nullptr;

	enum EnMovingUpDown{
		enMoving_Up,
		enMoving_Down,
		enMoving_Num
	};

	EnMovingUpDown movingSpeedState = enMoving_Up;
};

