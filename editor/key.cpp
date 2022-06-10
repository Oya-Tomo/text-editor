#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>
#include <conio.h>

#include "key.h"

using namespace std;

const vector<string> ctrlKeyList = {
	"<[ctrl-a]>",
	"<[ctrl-b]>",
	"<[ctrl-c]>",
	"<[ctrl-d]>",
	"<[ctrl-e]>",
	"<[ctrl-f]>",
	"<[ctrl-g]>",
	"<[ctrl-h]>",
	"<[ctrl-i]>",
	"<[ctrl-j]>",
	"<[ctrl-k]>",
	"<[ctrl-l]>",
	"<[ctrl-m]>",
	"<[ctrl-n]>",
	"<[ctrl-o]>",
	"<[ctrl-p]>",
	"<[ctrl-q]>",
	"<[ctrl-r]>",
	"<[ctrl-s]>",
	"<[ctrl-t]>",
	"<[ctrl-u]>",
	"<[ctrl-v]>",
	"<[ctrl-w]>",
	"<[ctrl-x]>",
	"<[ctrl-y]>",
	"<[ctrl-z]>",
};

const map<vector<int>, string> operationKeyList = {
	{ vector<int>({ 0x1b }), "esc]>" },
	{ vector<int>({ 224, 72 }), "up]>"},
	{ vector<int>({ 224, 75 }), "left]>"},
	{ vector<int>({ 224, 77 }), "right]>"},
	{ vector<int>({ 224, 80 }), "down]>"},
	{ vector<int>({ 0, 59 }), "f1]>"},
	{ vector<int>({ 0, 60 }), "f2]>"},
	{ vector<int>({ 0, 61 }), "f3]>"},
	{ vector<int>({ 0, 62 }), "f4]>"},
	{ vector<int>({ 0, 63 }), "f5]>"},
	{ vector<int>({ 0, 64 }), "f6]>"},
	{ vector<int>({ 0, 65 }), "f7]>"},
	{ vector<int>({ 0, 66 }), "f8]>"},
	{ vector<int>({ 0, 67 }), "f9]>"},
	{ vector<int>({ 0, 68 }), "f10]>"},
	{ vector<int>({ 224, 133 }), "f11]>"},
	{ vector<int>({ 224, 134 }), "f12]>"},
	{ vector<int>({ 224, 82 }), "insert]>"},
	{ vector<int>({ 224, 83 }), "delete]>"},
};


string keyBind() {
	int c;
	c = _getch();
	byte keyState[256];
	GetKeyboardState(keyState);
	bool shiftState = (bool)(GetAsyncKeyState(VK_SHIFT) & 0x8000);
	bool ctrlState = (bool)(GetAsyncKeyState(VK_CONTROL) & 0x8000);
	//bool shiftState = (keyState[VK_SHIFT] & 0x80);
	//bool ctrlState = (keyState[VK_CONTROL] & 0x80);

	vector<int> stdi;
	stdi.push_back(c);

	while (_kbhit() != 0) {
		c = _getch();
		stdi.push_back(c);
	}

	string stds(stdi.begin(), stdi.end());

	if (ctrlState && c <= 26) {
		return ctrlKeyList[(int)c - 1];
	}

	else if (stds == "\b") {
		return "<[back]>";
	}
	else if (stds == "\r") {
		return "<[enter]>";
	}
	else if (stds == "\t") {
		return "<[tab]>";
	}
	else if (operationKeyList.count(stdi)) {
		if (shiftState) {
			return "<[shift-" + operationKeyList.at(stdi);
		}
		else {
			return "<[" + operationKeyList.at(stdi);
		}
	}

	return stds;
}
