class PieceEffect;
class Effect;

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
	void Draw_Shock();
	static void PlayAnime(const double& PlayX, const double& PlayY, eExplosion_t type);
	static void PlaySpread(const double & PosX, const double & PosY, const double & ANGLE, eSpread_t type);
	static void PlayShock(double x, double y);

private:
	int img_shock;
	static PieceEffect* pieceef;
	static Effect** effect;
	static Shock s_shock[];
};