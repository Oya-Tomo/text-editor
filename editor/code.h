#pragma once
#ifndef CODE_H
#define CODE_H

#include <string>
#include <vector>

using namespace std;

class Code
{
public:
	Code();
	void setText(string content);
	vector<string> getText();
	vector<int> getViewSize();

	void renderCode(string keyEvent);
	void renderViewCode();
	void renderOneLineCode();

	void setColorMode(int mode);
	string getCopyText();

private:
	vector<string> text;
	int x;
	int y;
	int _x; // pool cursor x positon when you move cursor to right or left.
	//vector<int> charCounts;

	int poolingX;
	int poolingY;

	int colorMode;

	int top;
	void renderScrollUpView(int diff);
	void renderScrollDownView(int diff);
	void scrollView();
	void setRelativeCursorPos();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void moveLeftWithPool();
	void moveRightWithPool();
	void moveUpWithPool();
	void moveDownWithPool();

	void insertString(string content);
	void pressEnter();
	void pressBack();
	void pressDelete();
	void pressTab();

	void deleteRange();
	void poolXPosition();
	void poolPosition();
};

#endif // !CODE_H