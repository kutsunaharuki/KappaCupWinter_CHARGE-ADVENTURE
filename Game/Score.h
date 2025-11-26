#pragma once

enum class ScoreDigit : uint8_t{
	enZero = 0,
	enOne,
	enTwo,
	enThree,
	enFour,
	enFive,
	enSix,
	enSeven,
	enEight,
	enNine,
	enScore_Num
};


class Score : public IGameObject
{
public:
	ScoreDigit m_scores[static_cast<int>(ScoreDigit::enScore_Num)];

	bool Start()override;
	void Update()override;
	void AddScore(int value);
	void Render(RenderContext& rc)override;

	
	int m_realScore       = 0;      //ゲーム画面に出すスコア。
	int m_displayScore    = 0;      //表示用のスコア。
	float m_scoreAddSpeed = 400.0f; //n/s秒で何点増減するか
	const Vector3 m_startSpritePos = { 500.0f,400.0f,0.0f };
	ScoreDigit m_scoreNum[static_cast<int>(ScoreDigit::enScore_Num)];


private:
	SpriteRender m_scoreRender[10];
	FontRender m_scoreFontRender;


};

