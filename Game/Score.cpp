#include "stdafx.h"
#include "Score.h"

namespace {
	float W = 30.0f;
	float H = 28.0f;

	/** スプライト開始位置 */
	const Vector3 START_SPRITE_POS = { 500.0f,400.0f,0.0f };

	std::string FILE_FEAD_PATH = "Assets/Sprite/";
	std::string FILE_END_PATH = ".dds";


	struct ScoreInfo {
		std::string file = "file";

		std::string GetSpriteFile() {
			return FILE_FEAD_PATH + file + FILE_END_PATH;
		}
	};


	ScoreInfo numbers[static_cast<int>(ScoreDigit::enScore_Num)]
	{
		{"0"},{"1"},{"2"},{"3"},{"4"},
		{"5"},{"6"},{"7"},{"8"},{"9"}
	};

	/** 表示桁数(変更不可の値) */
	static const int DIGIT = 6;
}


bool Score::Start()
{
	/** 0~9のスプライト画像を読み込む(1文字ずつ) */
	for (int i = 0; i < 10; i++)
	{
		std::string file = numbers[i].GetSpriteFile();
		m_scoreRender[i].Init(file.c_str(), W, H);
	}
	return true;
}


void Score::Update()
{
	/** カウントアップアニメーション */
	if (m_displayScore < m_realScore)
	{
		float inverSe = g_gameTime->GetFrameDeltaTime();
		m_displayScore += (int)(m_scoreAddSpeed * inverSe);
		if (m_displayScore > m_realScore)
		{
			m_displayScore = m_realScore;
		}
	}
	for (int i = 0; i < static_cast<int>(ScoreDigit::enScore_Num); i++)
	{
		m_scoreRender[i].SetPosition(m_startSpritePos);
		m_scoreRender[i].Update();
	}
}


/** 敵を倒したときに呼ぶ */
void Score::AddScore(int value)
{
	m_realScore += value;
}


void Score::Render(RenderContext& rc)
{
	char scoreText[16];
	sprintf(scoreText, "%06d", m_displayScore);
	for (int i = 0; i < DIGIT; i++)
	{
		int num = scoreText[i] - '0';
		Vector3 pos = m_startSpritePos;
		pos.x += i * W;
		
		m_scoreRender[num].Draw(rc);
		/** 正しい数字を取得して描画 */
		//m_scoreRender[num].SetPosition(pos);

	}
}