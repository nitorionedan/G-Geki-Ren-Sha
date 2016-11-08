#pragma once
#include "Vector2D.hpp"
#include <vector>


class CycloneEffect
{
	struct tParam
	{
		int time;
		double radAng;
		double exrate;
		double distance;
		double dis_exrate, dis_distance; // Displacements
		Vector2D pos; // position
		bool isPlay;

		bool operator==(const tParam& other)
		{
			return time         == other.time         &&
				   exrate       == other.exrate       &&
				   distance     == other.distance     &&
				   dis_exrate   == other.dis_exrate   &&
				   dis_distance == other.dis_distance &&
				   isPlay       == other.isPlay;
		}
	};

public:
	CycloneEffect();
	~CycloneEffect();
	void Update();
	void Draw();
	void PlayAnime(double x, double y);
	void Create(double x, double y, double radian);

private:
	static int s_playNum; // Num of play

	std::vector <tParam> m_tParam;
	int gh_energy;
};