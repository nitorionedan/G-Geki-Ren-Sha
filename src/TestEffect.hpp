#pragma once

class TestEffect
{
public:
	TestEffect();
	~TestEffect();
	void Update();
	void Draw();
	void PlayAnime();

private:
	const double maxRadius, minRadius;

	double m_radius;
};