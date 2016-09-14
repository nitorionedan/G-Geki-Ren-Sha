/*
@referenced	http://dixq.net/rp/58.html
*/

#include "Stage1.hpp"
#include <cassert>
#include <algorithm>


namespace
{
	const VtPm_t t_vtpm[VertexNum] = {
		{ -1.f, 1.f, 0.f, 0.f },
		{ 1.f, 1.f, 1.f, 0.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ 1.f, -1.f, 1.f, 1.f },
		{ -1.f, -1.f, 0.f, 1.f },
		{ 1.f, 1.f, 1.f, 0.f }
	};
	float z;
	int ObjectNum;
}


Stage1::Stage1()
{
	int i = t_left_view.img = LoadGraph("GRAPH/mydat/img/kabe.png");
	gh[0] = LoadGraph("GRAPH/mydat/img/tex.png");
	gh[1] = LoadGraph("GRAPH/mydat/img/kabe.png");

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

	/*
	int ImgHandle   : �摜�n���h��
	int ImgSize     : �摜�T�C�Y
	int ImgX1       : �摜�̎g�p���镔��������W
	int ImgY1       : �摜�̎g�p���镔��������W
	int ImgX2       : �摜�̎g�p���镔���E�����W
	int ImgY2       : �摜�̎g�p���镔���E�����W
	float LargeX    : �`�悷��傫���i���j
	float LargeY    : �`�悷��傫���i�c�j
	int Type        : �`��^�C�v�@0:��ʂƓ��������@1:��ʂɐ����i�ǁj�@2:��ʂɐ����i�n�ʁj
	float FromZ     : �`����n�߂鉜�s��
	float FadeFromZ : �t�F�[�h�C�����n�߂鉜�s��
	float FadeToZ   : �t�F�[�h�A�E�g���n�߂鉜�s��
	float ToZ       : �`����I��鉜�s��
	float GraphX    : �`�悷�钆�S�_
	float GraphY    : �`�悷�钆�S�_
	int ObchildMax  : type��0�̏ꍇ�̂݁A�����ɂ����\�����邩
	*/

	ObjectNum = 0;
	IniObj(&Object[0], gh[0], 512, 0, 0, 256, 128, 250, 50, 2, 1000, 400, -200, -400, 320, 240 - 90, ObChildMax);
	IniObj(&Object[1], gh[0], 512, 60, 270, 405, 512, 180, 125, 0, 1000, 400, -200, -400, 470, 275, 6);
	IniObj(&Object[2], gh[1], 512, 0, 0, 390, 512, 73, 90, 1, 1000, 400, -200, -400, 170, 240, ObChildMax);
}


void Stage1::Update()
{
	CalcObject();
	SortObject();
}


void Stage1::Draw()
{
	/// SetDrawArea(FX, FY, FX + FMX, FY + FMY);
	SetDrawMode(DX_DRAWMODE_BILINEAR); // make smooth polygon
	for(auto Ob : Object)
	{
		for (int s = 0; s < Ob.childMax; ++s)
			DrawPolygon3D(Ob.child[s].Vertex, 2, Ob.img, TRUE);
	}
	SetDrawMode(DX_DRAWMODE_NEAREST); // reset default
	/// SetDrawArea(0, 0, 640, 480);
}


void Stage1::IniObj(Object_t * Ob, int ImgHandle, int ImgSize, int ImgX1, int ImgY1, int ImgX2, int ImgY2, float LargeX, float LargeY, int Type, float FromZ, float FadeFromZ, float FadeToZ, float ToZ, float GraphX, float GraphY, int Obchild_Max)
{
	if ( ObjectNum >= ObjectNumMax - 1 )
	{
		assert(!"out of limit");
		return;
	}
	++ObjectNum;

	Ob->img = ImgHandle;//�摜�n���h��
	Ob->imgSize = ImgSize;//�摜�T�C�Y
	Ob->imgX1 = ImgX1;
	Ob->imgY1 = ImgY1;
	Ob->imgX2 = ImgX2;
	Ob->imgY2 = ImgY2;
	Ob->largeX = LargeX;//�Ƃ肠�����`�悷��傫����K���ɐݒ�B�c�E����͑f�ނ̒ʂ�ɂ���
	Ob->largeY = LargeY;
	Ob->type = Type;//�^�C�v�𐂒���
	Ob->fromZ = FromZ;//�`��J�n�n�_
	Ob->fadeFromZ = FadeFromZ;//�`��t�F�[�h�C���J�n�n�_
	Ob->fadeToZ = FadeToZ;//�`��t�F�[�h�A�E�g�J�n�n�_
	Ob->toZ = ToZ;//�`��I���n�_
	Ob->childMax = ObChildMax;

	if (Ob->type == 0)
		Ob->childMax = Obchild_Max;

	if(Ob->childMax - 1 <= 0)
	{
		assert(!"Abnormality num of render");
		return;
	}

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

					if (Is_far)	// �ʒu���`�悷��͈͂�艓�������瓧�߂O 
						alpha = 0;
					else if (Is_fadeIn) //�i�߂Â��Ă���ꍇ�j�t�F�[�h�C������ʒu��������
						alpha = static_cast<unsigned char>( 255.f / (Object[t].fadeToZ - Object[t].toZ) * (z - Object[t].toZ) );					
					else if (Is_renderRange) // �ʏ�`�悷��ʒu�Ȃ�
						alpha = 255;
					else if (Is_fadeOut) //�i�߂Â��Ă���ꍇ�j�t�F�[�h�A�E�g����ʒu��������
						alpha = static_cast<unsigned char>(255.f / (Object[t].fromZ - Object[t].fadeFromZ) * (Object[t].fromZ - z));
					else if (Is_near)
						alpha = 0;
				}

				const bool& Is_imvisibleNear = ( Object[t].child[s].center.z < Object[t].toZ - Object[t].zWidth * 0.5f );
				const bool& Is_imvisibleFar = ( Object[t].child[s].center.z > Object[t].fromZ + Object[t].zWidth * 0.5f );
				
				if (Is_imvisibleNear) // �߂Â��Č����Ȃ��Ȃ�����
				{
					// ��Ԍ���������
					float sub = (Object[t].toZ - Object[t].zWidth * 0.5f) - Object[t].child[s].center.z;
					Object[t].child[s].center.z = Object[t].fromZ + Object[t].zWidth * 0.5f - sub;
				}
				else if (Is_imvisibleFar) // ���������Č����Ȃ��Ȃ�����
				{
					// ��Ԃ����瑤��
					float sub = Object[t].child[s].center.z - (Object[t].fromZ + Object[t].zWidth * 0.5f);
					Object[t].child[s].center.z = Object[t].toZ - Object[t].zWidth * 0.5f + sub;
				}
			}
		}
	}
}


void Stage1::SortObject()
{
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
