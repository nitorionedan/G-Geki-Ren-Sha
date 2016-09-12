#pragma once
#include "Field.hpp"
#include "Vector2D.hpp"
#include <array>
#include <DxLib.h>


class Stage1 : public Field
{
	static const int VertexNum = 6;

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

	// �O�p�`�̃|���S���Q�Ŏl�p�`��`�悷��ׂ̒l�B
	// ���l�Œ�Ȃ̂ŁA�o����K�v�Ȃ�
	struct VtPm_t
	{
		float x, y;
		float u, v;
	};

public:
	Stage1();
	~Stage1();
	void Initialize();
	virtual void Update() override;
	virtual void Draw() override;

private:
	View_t t_left_view;
	std::array<VtPm_t, VertexNum> t_vtpm;
};