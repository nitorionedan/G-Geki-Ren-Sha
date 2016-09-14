#pragma once
#include "Field.hpp"
#include "Vector2D.hpp"
#include <array>
#include <DxLib.h>

constexpr int VertexNum = 6;
constexpr int ObChildMax = 11;
constexpr int ObjectNumMax = 10;


// 三角形のポリゴン２つで四角形を描画する為の値。
// 数値固定なので、覚える必要なし
struct VtPm_t
{
	float x, y;
	float u, v;
};


class Stage1 : public Field
{
	struct View_t
	{
		bool isVertical;
		int img;
		VECTOR center;
		float large_x, large_y;
		float u, v;
		float from_z, to_z;
		float fade_from_z, fade_to_z;
		VERTEX_3D vertex[VertexNum];
	};

	/* struct about a texture */
	struct ObChild_t
	{
		VECTOR center;
		VERTEX_3D Vertex[VertexNum]; // texture's vertex
	};

	/* struct a lot of texture */
	struct Object_t
	{
		int type;
		int img;
		int imgSize;
		int imgX1, imgX2, imgY1, imgY2;
		float largeX, largeY;
		float zWidth;
		float fromZ, toZ;
		float fadeFromZ, fadeToZ;
		int childMax;
		ObChild_t child[ObChildMax];
	};

public:
	Stage1();
	~Stage1();
	void Initialize();
	virtual void Update() override;
	virtual void Draw() override;

private:
	void IniObj(Object_t *Ob, int ImgHandle, int ImgSize, int ImgX1, int ImgY1, int ImgX2, int ImgY2, float LargeX, float LargeY,
		int Type, float FromZ, float FadeFromZ, float FadeToZ, float ToZ, float GraphX, float GraphY, int Obchild_Max);
	void CalcObject();
	void SortObject();

	View_t t_left_view;
	Object_t Object[ObjectNumMax];
	int gh[2];
};