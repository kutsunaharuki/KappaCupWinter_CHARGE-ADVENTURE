#pragma once

class Warp;
class MovingFloor;
class SkyGround;
class Player:public IGameObject
{
public:
	Player();
	~Player();
	bool Start()override;
	void Update();
	void Move();
	void Rotation();
	void PlayAnimation();
	void ManageState();
    void Render(RenderContext& rc);

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	void  SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//�L�����N�^�[�R���g���[���[�̎擾�B
	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Jump,
		enAnimationClip_Walk,
		enAnimationClip_Run,
		enAnimationClip_Num
	};

	ModelRender m_modelRender;
	AnimationClip animationClips[enAnimationClip_Num];
	Vector3 m_position = Vector3::Zero;                 //���W�B
	Vector3 moveSpeed = Vector3::Zero;                  //�ړ����x�B
	Quaternion m_rot;                                   //��]�B


	Vector3 m_setPos = Vector3(0.0f, 300.0f, 0.0f);     //���[�v��B

	Vector3 m_fontPos = Vector3(100.0f, 300.0f, 0.0f);  //�t�H���g�̍��W�B
	FontRender m_posFontRender;                         //���W�̕`��B

	CharacterController m_charaCon;                     //�L�����N�^�[�R���g���[���[�̓����蔻��B
	
	float m_jumpTime = 0.0f;                            //�W�����v���Ă鎞�ԁB	
	
	bool isDead     = false;                            //�v���C���[�����S������؂�ւ��t���O�B
	bool isDash     = false;                            //�������瑖��ɕς��t���O�B
	bool chargeJump = true;                             //�`���[�W�W�����v�̃t���O�B
	bool isHit      = false;                            //�S�[�X�g�I�u�W�F�N�g�ɓ���������̃t���O�B
	bool canJump    = false;                            //�W�����v���Ă邩�̃t���O�B

private:
	Warp* m_warp = nullptr;

	int playerState    = 0;

	bool CanWarp();
	//bool canJump = true;                               //�؂�ւ��B
	MovingFloor* m_movingFloor = nullptr;
	SkyGround* m_skyGround = nullptr;
	
};

