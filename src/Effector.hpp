// エフェクト関連
#include "Effect.hpp"
#include "Explosion.hpp"
#include "ExplosionEffect.hpp"
#include "ExplosionS.hpp"
#include "PieceEffect.hpp"
#include "Piece.hpp"
#include "ChargeEffect.hpp"

#define EX_NUM 4


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