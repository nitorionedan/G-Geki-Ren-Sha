#pragma once


class Graphic
{
private:
	const int AllFontNum = 49; // フォントの数

public:
	int h_blueChar[28];
	int h_orangeChar[28];
	int h_blueNum[10];
	int h_orangeNum[10];

	int h_blueFonts[49];
	int h_orangeFonts[49];

public:
	Graphic();
	Graphic(int &Gr_Handle);
	~Graphic();
	void DrawMyString(const int PosiX, const int PosiY, char* sentence, const int space, const bool IsBlueColor);

	// 一応の完成
	void DrawMyString2(int PosiX, int PosiY, char* sentence, int space, bool IsBlueColor, double exrate);
	
	// まったく不本意だわ
	void DrawMyNumbers(const int PosiX, const int PosiY, char* sentence, const int space, const bool IsBlueColor);
	
	// まったく不本意だわ
	void DrawMyNumbersNotChar(const int PosiX, const int PosiY, int number, const int space);

	void DrawScore(int PosiX, int PosiY, int score, int space, double exrate);
};