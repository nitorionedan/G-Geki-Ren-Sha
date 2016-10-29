#include "SmokeEffect.hpp"

class PieceEffect;
class Effect;
class SpecialExplosionEffect;

enum eExplosion_t;
enum eSpread_t;


class Effector
{
	struct Shock
	{
		int img;
		bool isPlay;
		double x, y;
		double exrate;
	};

public:
	Effector();
	~Effector();
	void Update();
	void Draw();
	void Draw_Explosion();
	void Draw_Shock();
	void Draw_Smoke();
	void Draw_SpecialExplo();
	static void PlayAnime(const double& PlayX, const double& PlayY, eExplosion_t type);
	static void PlaySpread(const double & PosX, const double & PosY, const double & ANGLE, eSpread_t type);
	static void PlayShock(double x, double y);
	static void PlaySmoke(double x, double y, eSmokeColor colorType);
	static void PlaySpecialEplosion(double x, double y);

private:
	int img_shock;
	static PieceEffect* pieceef;
	static Effect** effect;
	static Shock s_shock[];
	static SmokeEffect* smoke;
	static SpecialExplosionEffect* special;
};