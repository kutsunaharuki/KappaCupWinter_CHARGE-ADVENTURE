#include "stdafx.h"
#include "ChargeItem.h"
#include "Player.h"

namespace {
	const char* CHARGE_ITEM_FILE_PATH = "Assets/modelData/ChargeItem.tkm";
	const float ITEM_LENGTH = 200.0f;
	const float ITEM_DEGREE = 10.0f;
}

ChargeItem::ChargeItem()
{
}

ChargeItem::~ChargeItem()
{
}

bool ChargeItem::Start()
{
	m_chargeItemModelRender.Init(CHARGE_ITEM_FILE_PATH);
	m_chargeItemModelRender.SetPosition(m_itemPos);
	m_chargeItemModelRender.SetRotation(m_itemRot);
	m_chargeItemModelRender.SetScale(m_itemScale);
	m_chargeItemModelRender.Update();
	return true;
}

void ChargeItem::Update()
{
	GetItem();
}


/** アイテムの設定 */
void ChargeItem::GetItem()
{
	/*float time = g_gameTime->GetFrameDeltaTime();*/
	if (!m_player) {
		m_player = FindGO<Player>("player");
		return;
	}
	
	//if (m_isDead) {
	//	DeleteGO(this);
	//	return;
	//}

	Vector3 playerPos = m_player->GetPosition();
	/** 距離計算 */
	Vector3 diff = playerPos - m_itemPos;
	if (diff.Length() <= ITEM_LENGTH) {
		m_player->m_itemCount += 1;
		DeleteGO(this);
	}
}


void ChargeItem::Render(RenderContext& rc)
{
	m_chargeItemModelRender.Draw(rc);
}