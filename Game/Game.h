#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class Warp;
class MovingFloor;
class MovingFloorUpDown;
class WarpHole;
class Enemy;
class ObstacleBox;
class Scaffolding;
class Asiba;
class SkyGround;
class StageGround;
class Poal;
class GameClear;
class GameOver;

class Game : public IGameObject
{
public:
	Game() {};
	~Game();
	bool Start();
	void Update();
	void TimeDraw();
	void Render(RenderContext& rc);

	//時間制限。
	Vector3 m_timerFontPos = { -100.0f,520.0f,0.0f };
	float m_timer = 120.0f;
	//時間の確認用の秒数。
	//float m_timer = 10.0f;
	FontRender m_timerFontRender;
	//-----------------------------------

	LevelRender m_levelRender;
	std::vector<StageGround*> m_stageGrounds;
	std::vector<Scaffolding*> m_scaffoldings;
	std::vector<SkyGround*> m_skyGrounds;
	std::vector<MovingFloor*> m_movingFloors;
	std::vector<MovingFloorUpDown*> m_movingFloorUpDowns;
	std::vector<Poal*> m_poals;

private:
	Player    * m_player       = nullptr;//プレイヤー。
	GameCamera* m_gameCamera   = nullptr;//ゲームカメラ。
	Warp      * m_warp         = nullptr;//ワープ。
	MovingFloor* m_movingFloor = nullptr;//動く床。
	WarpHole   * m_warpHole    = nullptr;//ワープボックス。
	//Stage1     * m_stage1      = nullptr;//ステージ1。
	Enemy      * m_enemy       = nullptr;//敵。
	ObstacleBox* m_obstacleBox = nullptr;//障害物(ボックス)。
	Scaffolding* m_scaffolding = nullptr;//足場。
	MovingFloorUpDown* m_upDown = nullptr;//動く床(Y軸アップ)。
	Asiba* m_asiba              = nullptr;//足場。
	SkyCube* m_skyCube = nullptr;//スカイキューブ。
	SkyGround* m_skyGround = nullptr;//重力のない足場。
	StageGround* m_stageGround = nullptr;//最初プレイヤーが触れる足場。
	Poal* m_poal = nullptr;//ゴールポール。
	GameClear* m_gameClear = nullptr;//ゲームクリア。
	GameOver* m_gameOver = nullptr;//ゲームオーバー。
	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();
	int m_skyCubeType = enSkyCubeType_DayToon_2;//昼間。
};

