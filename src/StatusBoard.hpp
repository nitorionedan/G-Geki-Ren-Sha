#pragma once

#include "Player.hpp"
#include "Graphics2D.hpp"

#include <array>



/// �Q�[�W�̃���������40����܂�
class StatusBoard : public Graphics2D
{
public:
	StatusBoard();
	~StatusBoard();
	void Update(const Player& player);
	void Draw(const Player& player);

private:
	const int POS_BOARD_X,
				POS_BOARD_Y;
	const int POS_BOMB_X,
				POS_BOMB_Y;
	const int POS_LIFE_X,
				POS_LIFE_Y;
	const int POS_KANJI_X,
				POS_KANJI_Y;
	const int POS_RENSHA_X,
				POS_RENSHA_Y;

	int gh_life,
		gh_bomb,
		gh_board,
		gh_rensha,
		gh_kanji[5];	// LoadDivGraph���������߁A���z��
};