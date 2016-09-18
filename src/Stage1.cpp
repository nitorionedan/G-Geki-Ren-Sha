/*
@referenced	http://dixq.net/rp/58.html
きこえたーきがーしたー
*/

#include "Stage1.hpp"
#include "Graphics2D.hpp"
#include <cassert>
#include <algorithm>
#include <DirectXMath.h>

constexpr int ObType_Horizon = 0;	// Horizontal
constexpr int ObType_VerWall = 1;	// Vertical(Wall)
constexpr int ObType_VerFloor = 2;	// Vertical(Floor)

namespace
{
	const VtPm_t t_vtpm[VertexNum] = 
	{
		{ -1.f, 1.f, 0.f, 0.f },
		{ 1.f, 1.f, 1.f, 0.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ 1.f, -1.f, 1.f, 1.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ 1.f, 1.f, 1.f, 0.f }
	};
	float z;
}


Stage1::Stage1()
{
	t_left_view.img = LoadGraph("GRAPH/mydat/img/kabe.png");
	gh[0] = LoadGraph("GRAPH/mydat/img/tex.png");
	gh[1] = LoadGraph("GRAPH/mydat/img/kabe.png");
	Screen = MakeScreen(640, 480, TRUE);
	SetFogEnable(TRUE);
	SetFogColor(255, 0, 0);
	SetFogStartEnd(0.f, 1000.f);

	Initialize();
}


Stage1::~Stage1()
{
	DeleteGraph(t_left_view.img);
	for (auto i : gh)
		DeleteGraph(i);
}


void Stage1::Initialize()
{
	::z = 0.f;
	ObjectNum = 0;

	/*
	int ImgHandle   : 画像ハンドル
	int ImgSize     : 画像サイズ
	int ImgX1       : 画像の使用する部分左上座標
	int ImgY1       : 画像の使用する部分左上座標
	int ImgX2       : 画像の使用する部分右下座標
	int ImgY2       : 画像の使用する部分右下座標
	float LargeX    : 描画する大きさ（横）
	float LargeY    : 描画する大きさ（縦）
	int Type        : 描画タイプ　0:画面と同じ向き　1:画面に垂直（壁）　2:画面に垂直（地面）
	float FromZ     : 描画を始める奥行き
	float FadeFromZ : フェードインを始める奥行き
	float FadeToZ   : フェードアウトを始める奥行き
	float ToZ       : 描画を終わる奥行き
	float GraphX    : 描画する中心点
	float GraphY    : 描画する中心点
	int ObchildMax  : typeが0の場合のみ、同時にいくつ表示するか
	*/
	int imgSize_tex = 0;
	int imgSize_wall = 0;
	GetGraphSize(gh[0], &imgSize_tex, &imgSize_tex);
	GetGraphSize(gh[1], &imgSize_wall, &imgSize_wall);
	IniObj(&Object[0], gh[0], imgSize_tex,  0,  0,   256, 128, 250, 50,  ObType_VerFloor, 1000, 400, -200, -400, 320, (240 - 90),  ObChildMax);
	IniObj(&Object[1], gh[0], imgSize_tex,  60, 270, 405, 512, 180, 125, ObType_Horizon,  1000, 400, -200, -400, 470,  275,        6);
	IniObj(&Object[2], gh[1], imgSize_wall, 0,  0,   390, 512, 73,  90,  ObType_Horizon,  1000, 400, -200, -400, 170,  240,        ObChildMax);
}


void Stage1::Update()
{
	CalcObject();
	SortObject();
}


void Stage1::Draw()
{
	int tmpScreen = GetDrawScreen();
	SetDrawScreen(Screen);
	ClearDrawScreen();

	SetDrawMode(DX_DRAWMODE_BILINEAR); // make smooth polygon
	for (auto Ob : Object)
	{
		for (int i = 0; i < Ob.childMax; ++i)
			DrawPolygon3D(Ob.child[i].Vertex, 2, Ob.img, TRUE);
	}
	SetDrawMode(DX_DRAWMODE_NEAREST); // reset default
	/// SetDrawArea(0, 0, 640, 480);

	SetDrawScreen(tmpScreen);
	DrawGraph(0, 0, Screen, TRUE);
	//DrawCircleScreen(320, 240, 200.f, 400.f, 50.f, Screen);

	/* Test */
	//DrawFormatString(0, 40, GetColor(255, 255, 0), "c_color=%d", c_color);
}


void Stage1::IniObj(Object_t * Ob, int ImgHandle, int ImgSize, int ImgX1, int ImgY1, int ImgX2, int ImgY2, float LargeX, float LargeY, int Type, float FromZ, float FadeFromZ, float FadeToZ, float ToZ, float GraphX, float GraphY, int Obchild_Max)
{
	if ( ObjectNum >= ObjectNumMax - 1 )
		assert(!"out of limit");

	++ObjectNum;

	Ob->img = ImgHandle;		// 画像ハンドル
	Ob->imgSize = ImgSize;		// 画像サイズ
	Ob->imgX1 = ImgX1;
	Ob->imgY1 = ImgY1;
	Ob->imgX2 = ImgX2;
	Ob->imgY2 = ImgY2;
	Ob->largeX = LargeX;		// とりあえず描画する大きさを適当に設定。縦・横比は素材の通りにする
	Ob->largeY = LargeY;
	Ob->type = Type;			// タイプを垂直に
	Ob->fromZ = FromZ;			// 描画開始地点
	Ob->fadeFromZ = FadeFromZ;	// 描画フェードイン開始地点
	Ob->fadeToZ = FadeToZ;		// 描画フェードアウト開始地点
	Ob->toZ = ToZ;				// 描画終了地点
	Ob->childMax = ObChildMax;

	if (Ob->type == 0)
		Ob->childMax = Obchild_Max;

	if(Ob->childMax - 1 <= 0)
		assert(!"Abnormality num of render");

	/* calc zWidth */
	Ob->zWidth = (Ob->fromZ - Ob->toZ) / (Ob->childMax - 1);

	float ou1 = static_cast<float>(Ob->imgX1) / Ob->imgSize,
		ou2 = static_cast<float>(Ob->imgX2 - Ob->imgX1) / Ob->imgSize;
	float ov1 = static_cast<float>(Ob->imgY1) / Ob->imgSize,
		ov2 = static_cast<float>(Ob->imgY2 - Ob->imgY1) / Ob->imgSize;
	
	for (int s = 0; s < Ob->childMax; ++s)
	{
		Ob->child[s].center.x = GraphX;
		Ob->child[s].center.y = GraphY;
		Ob->child[s].center.z = Ob->toZ - Ob->zWidth + Ob->zWidth * s;

		int i = 0;
		for(auto& Vertex : Ob->child[s].Vertex)
		{
			Vertex.r = Vertex.g = Vertex.b = Vertex.a = 255;
			Vertex.u = ou1 + ou2 * t_vtpm[i].u;
			Vertex.v = ov1 + ov2 * t_vtpm[i].v;
			++i;
		}
	}
}


void Stage1::CalcObject()
{
	for (int t = 0; t < ObjectNum; ++t)
	{
		for (int s = 0; s < Object[t].childMax; ++s)
		{
			Object[t].child[s].center.z -= 3;

			for (int i = 0; i < VertexNum; ++i)
			{
				VECTOR& pos = Object[t].child[s].Vertex[i].pos;
				VECTOR& center = Object[t].child[s].center;

				switch (Object[t].type)
				{
				case 0: // horizontal
					pos.x = center.x + Object[t].largeX * t_vtpm[i].x;
					pos.y = center.y + Object[t].largeY * t_vtpm[i].y;
					pos.z = center.z;
					break;
				case 1: // vertical (wall)
					pos.x = center.x;
					pos.y = center.y + Object[t].largeY * t_vtpm[i].y;
					pos.z = center.z + Object[t].zWidth / 2 * t_vtpm[i].x;
					break;				
				case 2: // vertical (floor)
					pos.x = center.x + Object[t].largeX * t_vtpm[i].x;
					pos.y = center.y;
					pos.z = center.z + Object[t].zWidth / 2 * t_vtpm[i].y;
					break;
				default: assert(!"Object[?].type is abnormal");
				}
			}
		}

		if (Object[t].fromZ - Object[t].fadeFromZ <= 0)
			assert(!"Object[t]ject[?].From is abnormal");
		else if (Object[t].fadeToZ - Object[t].toZ <= 0)
			assert(!"Object[t]ject[?].to is abnormal");
		else
		{
			for (int s = 0; s < Object[t].childMax; ++s)
			{
				for (int i = 0; i < VertexNum; ++i)
				{
					float z = Object[t].child[s].Vertex[i].pos.z;
					unsigned char& alpha = Object[t].child[s].Vertex[i].a;
					const bool& Is_far = ( z < Object[t].toZ );
					const bool& Is_fadeIn = ( Object[t].toZ < z && z <= Object[t].fadeToZ );
					const bool& Is_renderRange = ( Object[t].fadeToZ <= z && z <= Object[t].fadeFromZ );
					const bool& Is_fadeOut = ( Object[t].fadeFromZ <= z && z < Object[t].fromZ );
					const bool& Is_near = ( Object[t].fromZ < z );

					if (Is_far)	// 位置が描画する範囲より遠かったら透過０ 
						alpha = 0;
					else if (Is_fadeIn) //（近づいている場合）フェードインする位置だったら
						alpha = static_cast<unsigned char>( 255.f / (Object[t].fadeToZ - Object[t].toZ) * (z - Object[t].toZ) );					
					else if (Is_renderRange) // 通常描画する位置なら
						alpha = 255;
					else if (Is_fadeOut) //（近づいてくる場合）フェードアウトする位置だったら
						alpha = static_cast<unsigned char>(255.f / (Object[t].fromZ - Object[t].fadeFromZ) * (Object[t].fromZ - z));
					else if (Is_near)
						alpha = 0;
				}

				const bool& Is_invisibleNear = ( Object[t].child[s].center.z < Object[t].toZ - Object[t].zWidth * 0.5f );
				const bool& Is_invisibleFar = ( Object[t].child[s].center.z > Object[t].fromZ + Object[t].zWidth * 0.5f );
				VECTOR& center = Object[t].child[s].center;
				
				if (Is_invisibleNear) // 近づいて見えなくなったら
				{
					// 一番向こう側へ
					float sub = (Object[t].toZ - Object[t].zWidth * 0.5f) - center.z;
					center.z = Object[t].fromZ + Object[t].zWidth * 0.5f - sub;
				}
				else if (Is_invisibleFar) // 遠ざかって見えなくなったら
				{
					// 一番こちら側へ
					float sub = center.z - (Object[t].fromZ + Object[t].zWidth * 0.5f);
					center.z = Object[t].toZ - Object[t].zWidth * 0.5f + sub;
				}
			}
		}
	}

	
}


void Stage1::SortObject()
{
	/* Z sort */
	for (int t = 0; t < ObjectNum; ++t)
	{
		for (int i = 0; i < Object[t].childMax; ++i)
		{
			for (int j = i + 1; j < Object[t].childMax; ++j)
			{
				if (Object[t].child[i].center.z < Object[t].child[j].center.z)
					std::swap(Object[t].child[i], Object[t].child[j]);
			}
		}
	}
}