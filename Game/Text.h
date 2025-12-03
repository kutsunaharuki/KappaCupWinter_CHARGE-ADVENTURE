#pragma once
/** Ç±ÇÃÉQÅ[ÉÄÇÃScene */
enum SpriteType {
	enSpriteType_Title,
	enSpriteType_GameOver,
	enSpriteType_GameClear,
	enSpriteType_Pause,
	enSpriteType_Num
};

enum SceneType {
	enTitle,
	enGame,
	enGameOver,
	enGameClear,
	enPause
};

namespace nsSprite{
	const std::string FILE_PATH      = "Assets/Sprite/";
	const std::string FILE_EXTENTION = ".dds";

	static inline std::string GetFullPath(std::string fileName) {
		return FILE_PATH + fileName + FILE_EXTENTION;
	}
}