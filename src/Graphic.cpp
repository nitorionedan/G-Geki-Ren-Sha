#include "DxLib.h"

#include "Graphic.hpp"
#include <string.h>


Graphic::Graphic()
{
	LoadDivGraph("GRAPH/char_blue_7x9.png"  , 28, 7, 4, 7, 9, h_blueChar);
	LoadDivGraph("GRAPH/char_orange_7x9.png", 28, 7, 4, 7, 9, h_orangeChar);
	LoadDivGraph("GRAPH/num_blue_7x9.png"   , 10, 10, 1, 7, 9, h_blueNum);
	LoadDivGraph("GRAPH/num_orange_7x9.png" , 10, 10, 1, 7, 9, h_orangeNum);

	LoadDivGraph("GRAPH/blueFonts.png", 49, 7, 7, 7, 9, h_blueFonts);
	LoadDivGraph("GRAPH/orangeFonts.png", 49, 7, 7, 7, 9, h_orangeFonts);
}


Graphic::~Graphic()
{
	for (int i = 0; i < 28; i++)
	{
		DeleteGraph(h_blueChar[i]);
		DeleteGraph(h_orangeChar[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		DeleteGraph(h_blueNum[i]);
		DeleteGraph(h_orangeNum[i]);
	}
}


void Graphic::DrawMyString(const int PosiX, const int PosiY, char* sentence, const int space, const bool IsBlueColor)
{
	// 自作フォント画像の文字の番号に等しい
	int num[20];
	int charNum = strlen(sentence);

	//アスキーコードを０～９の数字に変換
	for (int i = 0; i < 26; i++)
	{
		for (int j = 0; j < charNum; j++)
		{
			if (sentence[j] == 65 + i)	num[j] = i;  // アルファベット
			if (sentence[j] == 33)		num[j] = 26; // 「！」
			if (sentence[j] == 63)		num[j] = 27; // 「？」
		}
	}

	// 文字の描画
	for (int i = 0; i < charNum; i++)
	{
		if (IsBlueColor)
		{
			DrawRotaGraph(PosiX + (i * space), PosiY, 1.0, 0.0, h_blueChar[num[i]], true);
			continue;
		}
		DrawRotaGraph(PosiX + (i * space), PosiY, 1.0, 0.0, h_orangeChar[num[i]], true);
	}
}


void Graphic::DrawMyString2(const int PosiX, const int PosiY, char* sentence, const int space, const bool IsBlueColor, const double exrate)
{
	// 自作フォント画像の文字の番号に等しい
	int num[500];
	int charNum = strlen(sentence);

	for (int i = 0; i < charNum; i++)
	{
		for (int j = 0; j < AllFontNum; j++)
		{
			// アルファベット 0番から
			for (int k = 0; k < 26; k++)
			{
				if (sentence[j] == 65 + k)
				{
					num[j] = k;
					break;
				}
			}

			// !
			if (sentence[j] == 33)
			{
				num[j] = 26;
				continue;
			}

			// ?
			if (sentence[j] == 63)
			{
				num[j] = 27;
				continue;
			}

			// 数字 29番から
			for (int k = 0; k < 10; k++)
			{
				if(sentence[j] == 48 + k)
				{
					num[j] = 28 + k;
					break;
				}
			}
			// % (39)
			if (sentence[j] == 37)
			{
				num[j] = 38;
				continue;
			}

			// ^ (40)
			if (sentence[j] == 94)
			{
				num[j] = 39;
				continue;
			}

			// (
			if (sentence[j] == 40)
			{
				num[j] = 40;
				continue;
			}

			// )
			if (sentence[j] == 41)
			{
				num[j] = 41;
				continue;
			}

			// - (ハイフン)
			if (sentence[j] == 45)
			{
				num[j] = 42;
				continue;
			}

			// ;
			if (sentence[j] == 59)
			{
				num[j] = 43;
				continue;
			}

			// ハート -> 小文字のh
			if (sentence[j] == 104)
			{
				num[j] = 44;
				continue;
			}

			// ほし -> 小文字のs
			if (sentence[j] == 115)
			{
				num[j] = 45;
				continue;
			}

			// ツイッターマーク -> 小文字のt
			if (sentence[j] == 116)
			{
				num[j] = 46;
				continue;
			}

			// きゅうり -> 小文字のk
			if (sentence[j] == 107)
			{
				num[j] = 47;
				continue;
			}

			// 空白
			if (sentence[j] == 32)	num[j] = 48;
		}
	}

	// 文字の描画
	for (int i = 0; i < charNum; i++)
	{
		// 青色
		if (IsBlueColor)
		{
			DrawRotaGraph(PosiX + (i * space), PosiY, exrate, 0.0, h_blueFonts[num[i]], true);
			continue;
		}

		// オレンジ
		DrawRotaGraph(PosiX + (i * space), PosiY, exrate, 0.0, h_orangeFonts[num[i]], true);
	}
}


void Graphic::DrawMyNumbers(const int PosiX, const int PosiY, char* sentence, const int space, const bool IsBlueColor)
{
	// 自作フォント画像の文字の番号に等しい
	int num[20];
	int charNum = strlen(sentence);

	// 数字の文字を数字フォント番号に置換
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < charNum; j++)
			if (sentence[j] == 48 + i)	num[j] = i; // 0 ~ 9 の数字
	}

	// 数字フォントの描画
	for (int i = 0; i < charNum; i++)
	{
		if (IsBlueColor)
		{
			DrawRotaGraph(PosiX + (i * space), PosiY, 1.0, 0.0, h_blueNum[num[i]], true);
			continue;
		}
		DrawRotaGraph(PosiX + (i * space), PosiY, 1.0, 0.0, h_orangeNum[num[i]], true);
	}
}


void Graphic::DrawMyNumbersNotChar(const int PosiX, const int PosiY, int number, const int space)
{
	
}


void Graphic::DrawScore(const int PosX, const int PosY, int score, const int space, const double exrate)
{
	for (int i = 0; i < 8; i++)
	{
		DrawRotaGraph(PosX - (space * i), PosY, exrate, 0.0, h_blueNum[score % 10], true);
		score /= 10;
	}
}