#define EX_NUM 4


class PieceEffect;
class Effect;

enum eExplosion_t;
enum eSpread_t;


class Effector
{
public:
	Effector();
	~Effector();
	void Update();
	void Draw();
	static void PlayAnime(const double& PlayX, const double& PlayY, eExplosion_t type);
	static void PlaySpread(const double & PosX, const double & PosY, const double & ANGLE, eSpread_t type);

private:
	static PieceEffect* pieceef;
	static Effect** effect;
};