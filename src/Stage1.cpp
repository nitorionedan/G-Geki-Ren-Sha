/*
@referenced	http://dixq.net/rp/58.html
*/

#include "Stage1.hpp"
#include <cassert>


namespace
{
	float z;
}


Stage1::Stage1()
{
	int i = t_left_view.img = LoadGraph("GRAPH/mydat/img/kabe.png");

	Initialize();
}


Stage1::~Stage1()
{
	DeleteGraph(t_left_view.img);
}


void Stage1::Initialize()
{
	::z = 0.f;

	t_vtpm[0] = { -1.f, 1.f, 0.f, 0.f };
	t_vtpm[1] = { 1.f, 1.f, 1.f, 0.f };
	t_vtpm[2] = { -1.f, -1.f, 0.f, 1.f };
	t_vtpm[3] = { 1.f, -1.f, 1.f, 1.f };
	t_vtpm[4] = { -1.f, -1.f, 0.f, 1.f };
	t_vtpm[5] = { 1.f, 1.f, 1.f, 0.f };

	t_left_view.large_x = 48.f;
	t_left_view.large_y = 60.f;
	t_left_view.isVertical = false;
	t_left_view.center.x = 220.f;
	t_left_view.center.y = 240.f;
	t_left_view.center.z = 0.f;
	t_left_view.u = 0.763671875f;
	t_left_view.v = 1.f;
	t_left_view.from_z      = 400;
	t_left_view.fade_from_z = 100;
	t_left_view.fade_to_z   = -100;
	t_left_view.to_z        = -400;

	int j = 0;
	for(auto& i : t_left_view.vertex)
	{
		i.r = 255;
		i.b = 255;
		i.g = 255;
		i.a = 255;
		i.u = t_left_view.u * t_vtpm[j].u;
		i.v = t_left_view.v * t_vtpm[j].v;
		++j;
	}
}


void Stage1::Update()
{
	t_left_view.center.z = ::z;

	if (CheckHitKey(KEY_INPUT_Z) > 0)
		::z += 1.4f;
	if (CheckHitKey(KEY_INPUT_Y) > 0)
		::z -= 1.4f;

	// vertical or horizontal
	if(t_left_view.isVertical)
	{
		int j = 0;
		for(auto& i : t_left_view.vertex)
		{
			i.pos.x = t_left_view.center.x + t_left_view.large_x * t_vtpm[j].x;
			i.pos.y = t_left_view.center.y + t_left_view.large_y * t_vtpm[j].y;
			i.pos.z = t_left_view.center.z;
			++j;
		}
	}
	else
	{
		int j = 0;
		for (auto& i : t_left_view.vertex)
		{
			i.pos.x = t_left_view.center.x;
			i.pos.y = t_left_view.center.y + t_left_view.large_y * t_vtpm[j].y;
			i.pos.z = t_left_view.center.z + t_left_view.large_x * t_vtpm[j].x;
			++j;
		}
	}

	/*
	z
	Object.FromZ        200
	z
	Object.FadeFromZ    100
	z
	Object.FadeToZ     -100
	z
	Object.ToZ         -200
	z
	*/
	if (t_left_view.from_z - t_left_view.fade_from_z <= 0.f)
		assert(!"Stage1::Update()");
	else if(t_left_view.fade_to_z - t_left_view.to_z <= 0.f)
		assert(!"Stage1::Update()");
	else
	{
		for(auto& i : t_left_view.vertex)
		{
			float z = i.pos.z;

			//位置が描画する範囲より遠かったら透過0
			if (z < t_left_view.to_z)
				i.a = 0;
			//(近づいている場合)フェードインする位置だったら
			else if (t_left_view.to_z < z && z <= t_left_view.fade_to_z)
				i.a = static_cast<unsigned char>(255.f / (t_left_view.fade_to_z - t_left_view.to_z) * (z - t_left_view.to_z));
			//通常描画する位置なら
			else if (t_left_view.fade_to_z <= z && z <= t_left_view.fade_from_z)
				i.a = 255;
			//(近づいてる場合)フェードアウトする位置だったら
			else if (t_left_view.fade_from_z <= z && z < t_left_view.from_z)
				i.a = static_cast<unsigned char>(255.f / (t_left_view.from_z - t_left_view.fade_from_z) * (t_left_view.from_z - z));
			//描画する範囲より近かったら透過0
			else if (t_left_view.from_z < z)
				i.a = 0.f;
		}
	}
}


void Stage1::Draw()
{
	DrawPolygon3D(t_left_view.vertex, 2, t_left_view.img, TRUE);
	DrawFormatString(0, 30, GetColor(255, 0, 255), "z =%f", t_left_view.center.z);
}
