#pragma once
class Warp;
class WarpHole:public IGameObject
{
public:
	WarpHole() {};
	~WarpHole() {};
	bool Start()override;
	void Update();
	void Render(RenderContext& rc);

	ModelRender m_warpHoleRender;
	Vector3 m_position = Vector3(100.0, 170.0f, 200.0f);
	Vector3 m_scale = Vector3(1.3f, 9.2f, 1.2f);
private:
	Warp* m_warp = nullptr;
};

