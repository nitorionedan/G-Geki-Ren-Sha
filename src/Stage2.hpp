#pragma once
#include "Field.hpp"
#include "Vector2D.hpp"
#include "Image.hpp"
#include <memory>
#include <array>


class Stage2 : public Field
{
public:
	Stage2();
	~Stage2();
	virtual void Update() override;
	virtual void Draw() override;

private:
	void Initialize();
	void Update_moon();
	const double ForeSpeed, BackSpeed, MidSpeed;

	std::unique_ptr<Image> img;
	std::array<Vector2D, 2> fore_pos, back_pos, mid_pos;
	Vector2D moon_pos;
	bool isMoonTime;
	double c_moon;
	double c_mid;
	double c_cloud_bright;
	double c_alpha_back;
	double c_alpha_mid;
	double c_alpha_fore;
};