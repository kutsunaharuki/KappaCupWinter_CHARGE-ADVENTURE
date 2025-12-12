#include "stdafx.h"
#include "PauseScene.h"

namespace {
    std::string PAUSE_FEAD_PATH = "Assets/Sprite/PAUSE/";
    std::string PAUSE_END_PATH  = ".dds";


    struct PauseInfo {
        std::string spritePath;


        Vector3 pos;
        Vector3 scale;
        Vector4 alpha;

        float Wide;
        float height;

        std::string GetFullPath() {
            return PAUSE_FEAD_PATH + spritePath + PAUSE_END_PATH;
        };
    };


    PauseInfo pauseScreens[enSprite_Num] {
        {
            "PauseColor"
            , {0.0f,  0.0f,  0.0f       }
            , {1.0f,  1.0f,  1.0f       }
            , {1.0f,  1.0f,  1.0f,  0.4f}
            , 1920.0f
            , 1080.0f
        },

        {
             "Pause"
            , {0.0f, 400.0f,0.0f      }
            , {1.2f, 1.2f, 1.2f       }
            , {1.0f, 1.0f, 1.0f,  1.0f}
            , 486.0f
            , 131.0  
        },
        
        {
             "Resume"
            , {0.0f, 150.0f,0.0f  }
            , {1.4f,1.4f,1.4f     }
            , {1.0f,1.0f,1.0f,1.0f}
            , 268.0f
            , 61.0f  
        },
        
        {
              "Quite"
             , {0.0f, 100.0f,0.0f  }
             , {1.4f,1.4f,1.4f     }
             , {1.0f,1.0f,1.0f,1.0f}
             , 158.0f
             , 71.0f  
        }
    };
}


bool PauseScene::Start()
{
    PauseInfo* info = pauseScreens;
    for (int i = 0; i < enSprite_Num; i++) {
        std::string spriteFile = info[i].GetFullPath();
        m_pauseSpriteRender[i].Init(spriteFile.c_str(), info[i].Wide, info[i].height);
        m_pauseSpriteRender[i].SetPosition(info[i].pos);
        m_pauseSpriteRender[i].SetScale(info[i].scale);
        m_pauseSpriteRender[i].SetMulColor(info[i].alpha);
        m_pauseSpriteRender[i].Update();
    }

    return true;
}


void PauseScene::Update()
{
    
}


void PauseScene::Render(RenderContext& rc)
{
    for (int k = 0; k < enSprite_Num; k++) {
        m_pauseSpriteRender[k].Draw(rc);
    }
}
