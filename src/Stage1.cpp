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

	/*t_left_view.large_x = 48.f;
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
	}*/

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
	//t_left_view.center.z = ::z;

	//if (CheckHitKey(KEY_INPUT_Z) > 0)
	//	::z += 1.4f;
	//if (CheckHitKey(KEY_INPUT_Y) > 0)
	//	::z -= 1.4f;

	//// vertical or horizontal
	//if(t_left_view.isVertical)
	//{
	//	int j = 0;
	//	for(auto& i : t_left_view.vertex)
	//	{
	//		i.pos.x = t_left_view.center.x + t_left_view.large_x * t_vtpm[j].x;
	//		i.pos.y = t_left_view.center.y + t_left_view.large_y * t_vtpm[j].y;
	//		i.pos.z = t_left_view.center.z;
	//		++j;
	//	}
	//}
	//else
	//{
	//	int j = 0;
	//	for (auto& i : t_left_view.vertex)
	//	{
	//		i.pos.x = t_left_view.center.x;
	//		i.pos.y = t_left_view.center.y + t_left_view.large_y * t_vtpm[j].y;
	//		i.pos.z = t_left_view.center.z + t_left_view.large_x * t_vtpm[j].x;
	//		++j;
	//	}
	//}

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
	//if (t_left_view.from_z - t_left_view.fade_from_z <= 0.f)
	//	assert(!"Stage1::Update()");
	//else if(t_left_view.fade_to_z - t_left_view.to_z <= 0.f)
	//	assert(!"Stage1::Update()");
	//else
	//{
	//	for(auto& i : t_left_view.vertex)
	//	{
	//		float z = i.pos.z;

	//		//�ʒu���`�悷��͈͂�艓�������瓧��0
	//		if (z < t_left_view.to_z)
	//			i.a = 0;
	//		//(�߂Â��Ă���ꍇ)�t�F�[�h�C������ʒu��������
	//		else if (t_left_view.to_z < z && z <= t_left_view.fade_to_z)
	//			i.a = static_cast<unsigned char>(255.f / (t_left_view.fade_to_z - t_left_view.to_z) * (z - t_left_view.to_z));
	//		//�ʏ�`�悷��ʒu�Ȃ�
	//		else if (t_left_view.fade_to_z <= z && z <= t_left_view.fade_from_z)
	//			i.a = 255;
	//		//(�߂Â��Ă�ꍇ)�t�F�[�h�A�E�g����ʒu��������
	//		else if (t_left_view.fade_from_z <= z && z < t_left_view.from_z)
	//			i.a = static_cast<unsigned char>(255.f / (t_left_view.from_z - t_left_view.fade_from_z) * (t_left_view.from_z - z));
	//		//�`�悷��͈͂��߂������瓧��0
	//		else if (t_left_view.from_z < z)
	//			i.a = 0.f;
	//	}
	//}

	CalcObject();
	SortObject();
}


void Stage1::Draw()
{
	//DrawPolygon3D(t_left_view.vertex, 2, t_left_view.img, TRUE);
	//DrawFormatString(0, 30, GetColor(255, 0, 255), "z =%f", t_left_view.center.z);

	/// SetDrawArea(FX, FY, FX + FMX, FY + FMY);
	SetDrawMode(DX_DRAWMODE_BILINEAR); // make smooth polygon
	for(auto Ob : Object)
	{
		for (int s = 0; s < Ob.childMax; ++s)
			DrawPolygon3D(Ob.child[s].Vertex, 2, Ob.img, TRUE);
	}
	int num = 1;
	DrawFormatString(30, 30, GetColor(255, 0, 0), "Object[%d].child[%d].Vertex[%d].a : %u", num, num, num, static_cast<unsigned int>(Object[num].child[num].Vertex[num].a));
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

		for (int i = 0; i < VertexNum; ++i)
		{
			Ob->child[s].Vertex[i].r = Ob->child[s].Vertex[i].g = Ob->child[s].Vertex[i].b = Ob->child[s].Vertex[i].a = 255;
			Ob->child[s].Vertex[i].u = ou1 + ou2 * t_vtpm[i].u;
			Ob->child[s].Vertex[i].v = ov1 + ov2 * t_vtpm[i].v;
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
				switch (Object[t].type)
				{
				case 0: // horizontal
					Object[t].child[s].Vertex[i].pos.x = Object[t].child[s].center.x + Object[t].largeX * t_vtpm[i].x;
					Object[t].child[s].Vertex[i].pos.y = Object[t].child[s].center.y + Object[t].largeY * t_vtpm[i].y;
					Object[t].child[s].Vertex[i].pos.z = Object[t].child[s].center.z;
					break;

				case 1: // vertical (wall)
					Object[t].child[s].Vertex[i].pos.x = Object[t].child[s].center.x;
					Object[t].child[s].Vertex[i].pos.y = Object[t].child[s].center.y + Object[t].largeY * t_vtpm[i].y;
					Object[t].child[s].Vertex[i].pos.z = Object[t].child[s].center.z + Object[t].zWidth / 2 * t_vtpm[i].x;
					break;
				
				case 2: // vertical (floor)
					Object[t].child[s].Vertex[i].pos.x = Object[t].child[s].center.x + Object[t].largeX * t_vtpm[i].x;
					Object[t].child[s].Vertex[i].pos.y = Object[t].child[s].center.y;
					Object[t].child[s].Vertex[i].pos.z = Object[t].child[s].center.z + Object[t].zWidth / 2 * t_vtpm[i].y;
					break;
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

					/* �ʒu���`�悷��͈͂�艓�������瓧�߂O */
					if (z < Object[t].toZ)
						Object[t].child[s].Vertex[i].a = 0;
					/* �i�߂Â��Ă���ꍇ�j�t�F�[�h�C������ʒu�������� */
					else if (Object[t].toZ < z && z <= Object[t].fadeToZ)
						Object[t].child[s].Vertex[i].a = static_cast<unsigned char>(255.f / Object[t].fadeToZ - Object[t].toZ) * (z - Object[t].toZ);
					/* �ʏ�`�悷��ʒu�Ȃ� */
					else if (Object[t].fadeToZ <= z && z <= Object[t].fadeFromZ)
						Object[t].child[s].Vertex[i].a = 255;
					/* �i�߂Â��Ă���ꍇ�j�t�F�[�h�A�E�g����ʒu�������� */
					else if (Object[t].fromZ < z)
						Object[t].child[s].Vertex[i].a = 0;
				}

				// �߂Â��Č����Ȃ��Ȃ�����
				if(Object[t].child[s].center.z < Object[t].toZ - Object[t].zWidth * 0.5f)
				{
					// ��Ԍ���������
					float sub = (Object[t].toZ - Object[t].zWidth * 0.5f) - Object[t].child[s].center.z;
					Object[t].child[s].center.z = Object[t].fromZ + Object[t].zWidth * 0.5f - sub;
				}
				else if (Object[t].child[s].center.z > Object[t].fromZ + Object[t].zWidth * 0.5f)
				{
					// ��Ԃ����瑤��
					float sub = Object[t].child[s].center.z - (Object[t].fromZ + Object[t].zWidth * 0.5f);
					Object[t].child[s].center.z = Object[t].toZ - Object[t].zWidth * 0.5f + sub;
				}
			}
		}
	}
}


void Stage1::SwapObChild(ObChild_t * Ob1, ObChild_t * Ob2){
	std::swap(*Ob1, *Ob2);
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
					SwapObChild(&Object[t].child[i], &Object[t].child[j]);
			}
		}
	}
}
