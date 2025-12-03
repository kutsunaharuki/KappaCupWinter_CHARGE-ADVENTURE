#pragma once
class Enemy;
class MovingFloor;
class MovingFloorUpDown;
class Scaffolding;
class Asiba;
class SkyGround;
class StageGround;
class Poal;
class BossStage;

class Stage  : public IGameObject
{
public:
	Stage();
	~Stage();

	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;


protected:
	/** SetLevel()ÇÃå„ÇÎÇÃ0ÇÕâΩÇ‡èëÇ¢ÇƒÇ»Ç©Ç¡ÇΩÇÁÉGÉâÅ[ */
	virtual void SetLevel() = 0;
	LevelRender m_levelRender;
	std::vector<Enemy*> m_frogs;
	std::vector<Enemy*> m_skeltons;
	std::vector<Enemy*> m_bosses;
	std::vector<MovingFloor*> m_movings;
	std::vector<MovingFloorUpDown*> m_upDowns;
	std::vector<Scaffolding*> m_scaffols;
	std::vector<SkyGround*> m_skyGrounds;
	std::vector<StageGround*> m_stageGrounds;
	std::vector<Asiba*> m_asibas;
	std::vector<StageGround*> m_grounds;
	std::vector<Poal*> m_poals;
	std::vector<BossStage*> m_bossStagies;
	
};

class FirstStage :public Stage
{
public:
	FirstStage();
	~FirstStage();


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	void SetLevel()override;
};


class LastStage : public Stage
{
public:
	LastStage();
	~LastStage();


	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc)override;

	void SetLevel()override;
};