#pragma once

#include "Level3DRender/LevelRender.h"

class Player;
class GameCamera;
class MovingFloor;
class MovingFloorUpDown;
class Enemy;
class ObstacleBox;
class Scaffolding;
class Asiba;
class SkyGround;
class StageGround;
class Poal;
class GameClear;
class GameOver;
class BossStage;
class HPUI;
class Score;
class SecondGround;
class SinkScaffold;
class RouteC;
class ChargeItem;
class Stage;
class Stage1;
class WarpBox;
class PauseScene;
class ChargeBar;
class CrossRoad;
class FallBox;

//ステージ遷移の場所。
enum class GameState {
	Stage1,            //1-1のステージ。
	Stage2,            //1-2のステージ。
	StageTransition,   //遷移中(タイマー停止)。
	BossStage,         //ボスステージ。
	enGameState_None = -1
};

class Game : public IGameObject
{
public:
	Game() {};
	~Game();
	bool Start();
	void Update();
	void TimeDraw();
	void ScoreDraw();
	void Render(RenderContext& rc);

	void CreateStage1();
	void DeleteStage1();

	void CreateStage2();
	void DeleteStage2();

	void CreateBossStage();
	void DeleteBossStage();

	/*bool IsTimeUp()
	{
		return m_isTimeUp;
	}*/
	

private:
	bool m_isTimeUp = false;
	Stage1* m_stage1 = nullptr;
	static bool m_isGoal;
	static bool m_isPause;
	static bool m_isNextGoal;

	static bool m_isWarp;
	
	static bool m_isBossKill;
public:
	static const bool GetIsGoal(){
		return m_isGoal;
	}

	static void SetIsGoal(bool isFlag) {
		m_isGoal = isFlag;
	}

	static const bool GetIsWarp() {
		return m_isWarp;
	}

	static void SetIsWarp(bool flag) {
		m_isWarp = flag;
	}


	static const bool GetIsNextGoal() {
		return m_isNextGoal;
	}

	///** 次のゴールのフラグ */
	static void SetIsNextGoal(bool flag) {
		m_isNextGoal = flag;
	}

	/** フラグを反転させる */
	static void ChangePause() {
		m_isPause = !m_isPause;
	}


	static const bool GetIsPause() {
		return m_isPause;
	}

	static void SetIsPause(bool isStop) {
		m_isPause = isStop;
	}

	static const bool GetIsBossKill() {
		return m_isBossKill;
	}

	static void SetIsBossKill(bool killFlag) {
		m_isBossKill = killFlag;
	}


	static GameState m_gameState;
	
private:
	//アイテム。
	void ItemDraw();
	Vector3 m_itemFontPos = { 400.0f,500.0f,0.0f };
	FontRender m_itemFontRender;

	//時間制限。
	Vector3 m_timerFontPos = { -100.0f,520.0f,0.0f };
	float m_timer = 120.0f;
	const float GetTimer()const { return m_timer; }
	//時間の確認用の秒数。
	//float m_timer = 10.0f;
	FontRender m_timerFontRender;
	//-----------------------------------
	
	LevelRender m_levelRender;
	LevelRender m_secondLevelRender;
	LevelRender m_bossLevelRender;

	//後に数が決まった時にstd::array<T, number*>~~;とする。
	std::vector<Enemy*> m_frogs;
	std::vector<Enemy*> m_skeltons;
	std::vector<MovingFloor*> m_movings;
	std::vector<MovingFloorUpDown*> m_upDowns;
	std::vector<Poal*> m_poals;


	std::vector<FallBox*> m_fallBoxs;
	std::vector<RouteC*> m_routeCs;
	std::vector<CrossRoad*> m_crossRoads;
	std::vector<SecondGround*> m_secondGrounds;
	std::vector<SinkScaffold*> m_sinkScaffolds;
	std::vector<ChargeItem*> m_chargeItems;
	std::vector<WarpBox*> m_warpBoxs;

	/** 
	 * std::array<T*, n>v 
	 * ↑
	 * 固定長配列[v]
	 */
	//std::array<Enemy*, 5>m_enemies;

private:
	Player    * m_player       = nullptr;//プレイヤー。
	GameCamera* m_gameCamera   = nullptr;//ゲームカメラ。
	MovingFloor* m_movingFloor = nullptr;//動く床。
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
	BossStage* m_bossStage = nullptr;//ボスステージ。
	HPUI* m_hpui = nullptr;//UI。
	Score* m_score = nullptr;//スコア。
	WarpBox* m_warpBox = nullptr;//ワープ。
	CrossRoad* m_crossRoad = nullptr;//分かれ道。

	PauseScene* m_pauseScene = nullptr;//ポーズ画面。
	ChargeBar* m_chargeBar = nullptr;//チャージバー。

	SecondGround* m_secondGround = nullptr;
	SinkScaffold* m_sinkScaffold = nullptr;
	RouteC* m_routeC = nullptr;
	ChargeItem* m_chargeItem = nullptr;
	FallBox* m_fallBox = nullptr;

	SoundSource* m_gameOverSe      = nullptr;//ゲームオーバーの音。
	SoundSource* m_gameClearSe     = nullptr;//ゲームクリアの音。
public:
	SoundSource* m_gameBGM         = nullptr;


private:
	//InGameTime* m_inGameTime = nullptr;//残り時間。
	/// <summary>
	/// 空を初期化。
	/// </summary>
	void InitSky();
	int m_skyCubeType = enSkyCubeType_DayToon;//昼間。
};

