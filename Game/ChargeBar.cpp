#include "stdafx.h"
#include "ChargeBar.h"

namespace {
    const int SMALL_DECREASE_VAL = 1;
    const int BIG_DECREASE_VAL = 2;

    std::string FILE_PATH = "Assets/Sprite/Charge/";
    std::string FILE_EXTENTION = ".dds";


    struct ChargeInfo {
        std::string chargeFilePath;

        Vector3 pos;
        Vector3 scale;
        Vector2 pivot;
        Vector4 alpha;

        float wide;
        float height;

        std::string GetFullPath() {
            return FILE_PATH + chargeFilePath + FILE_EXTENTION;
        }
    };

    ChargeInfo charges[enCharge_Num]{
        {
              "ChargeStageZero"
            , {-700.0f,-400.0f,0.0f}
            , {1.0f,0.6f,0.0f}
            , {0.0f,0.5f}
            , {1.0f,1.0f,1.0f,0.2f}
            , 575.0f
            , 149.0f
        },

        {
              "ChargeStageOne"
            , {-600.0f,-400.0f,0.0f}
            , {1.0f,0.6f,0.0f}
            , {0.0f,0.5f}
            , {1.0f,1.0f,1.0f,1.0f}
            , 99.0f
            , 152.0f
        },

        {
              "ChargeStageTwo"
            , {-500.0f,-400.0f,0.0f}
            , {1.0f,0.6f,0.0f}
            , {0.0f,0.5}
            , {1.0f,1.0f,1.0f,1.0f}
            , 99.0f
            , 152.0f
        },

        {
              "ChargeStageThree"
            , {-400.0f,-400.0f,0.0f}
            , {1.0f,0.6f,0.0f}
            , {0.0f,0.5f}
            , {1.0f,1.0f,1.0f,1.0f}
            , 99.0f
            , 152.0f
        },

        {
              "ChargeStageFour"
            , {-300.0f,-400.0f,0.0f}
            , {1.0f,0.6f,0.0f}
            , {0.0f,0.5}
            , {1.0f,1.0f,1.0f,1.0f}
            , 99.0f
            , 152.0f
        },

        {
              "ChargeTank"
            , {-200.0f,-400.0f,0.0f}
            , {1.0f,0.6f,0.0f}
            , {0.0f,0.5f}
            , {1.0f,1.0f,1.0f,1.0f}
            , 99.0f
            , 152.0f
        }
    };
}

bool ChargeBar::Start()
{
    m_isInited = true;
    //ChargeInfo* info = charges;
    for (int i = 0; i < enCharge_Num; i++) {
        auto& info = charges[i];
        std::string chargeFile = info.GetFullPath();
        m_chargeBarSpriteRender[i].Init(chargeFile.c_str(), info.wide, info.height);
        m_chargeBarSpriteRender[i].SetPosition(info.pos);
        m_chargeBarSpriteRender[i].SetScale(info.scale);
        m_chargeBarSpriteRender[i].SetPivot(info.pivot);
        m_chargeBarSpriteRender[i].SetMulColor(info.alpha);
        m_chargeBarSpriteRender[i].Update();
    }
    return true;
}

void ChargeBar::Update()
{
    UpdateChargeSprite();
}


void ChargeBar::AddChargeAmount()
{
    int next = static_cast<int>(chargeSprites) + 1;

    if (next >= enCharge_Num) {
        next = enCharge_Num - 1;
    }

    chargeSprites = static_cast<ChargeStage>(next);
    UpdateChargeSprite();
}


void ChargeBar::DecreaseChargeAmount(int value)
{
    int n = static_cast<int>(chargeSprites) - value;
    if (n < 0) {
        n = 0;
    }
    chargeSprites = static_cast<ChargeStage>(n);
    UpdateChargeSprite();
}


void ChargeBar::SmallDecreaseChargeAmount()
{
    DecreaseChargeAmount(SMALL_DECREASE_VAL);
}


void ChargeBar::BigDecreaseChargeAmounrt()
{
    DecreaseChargeAmount(BIG_DECREASE_VAL);
}


void ChargeBar::UpdateChargeSprite()
{
    int idx = static_cast<int>(chargeSprites);
    for (int i = 0; i < enCharge_Num; i++) {
        if (i <= idx) {
            m_chargeBarSpriteRender[i].SetMulColor(m_show);

            //満タンなら、満タンのスプライトだけ特殊な色にする。
            if (i == enCharge_Num - 1 && idx == enCharge_Num - 1) {
                m_chargeBarSpriteRender[i].SetMulColor(m_maxFlashColor);
            }
        }
        else {
            //現在の数より大きいスプライトは「非表示」にする。
            m_chargeBarSpriteRender[i].SetMulColor(m_hidden);
        }

        //色の更新。
        m_chargeBarSpriteRender[i].Update();
    }


    //HideAllSprites();
    //ShowCurrentSprite();
    //ApplyChargeState();
}


void ChargeBar::HideAllSprites()
{
    for (int i = 0; i < enCharge_Num; i++) {
        m_chargeBarSpriteRender[i].SetMulColor(m_hidden);
    }
}


void ChargeBar::ShowCurrentSprite()
{
    int idx = static_cast<int>(chargeSprites);
    m_chargeBarSpriteRender[idx].SetMulColor(m_show);
}


void ChargeBar::ApplyChargeState()
{
    int idx = static_cast<int>(chargeSprites);
    
    if (idx < 0 || idx >= enCharge_Num)return;

    if (idx == enCharge_Num - 1) {
        m_chargeBarSpriteRender[idx].SetMulColor(m_maxFlashColor);
    }
}


void ChargeBar::Render(RenderContext& rc)
{
    if (!m_isInited) return;

    /** 現在のチャージ段階(0~5まで)を取得 */
    int idx = static_cast<int>(chargeSprites);

    for (int i = 0; i <= idx; i++) {
        if (i >= 0 && i < enCharge_Num) {
            m_chargeBarSpriteRender[i].Draw(rc);
        }
    }
}
