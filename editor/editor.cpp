#include <iostream>
#include <numeric>
#include <string>
#include <Shlwapi.h>
#include <Windows.h>
#include <regex>

#include "code.h"
#include "color.h"
#include "core.h"
#include "key.h"
#include "utils.h"
#include "editor.h"

using namespace std;

static bool isCloseMode = false;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "File name not found !" << std::endl;
		exit(1);
	}
	else if (argc > 2) {
		std::cerr << "Too many args !" << std::endl;
		exit(1);
	}

	string filename = string{argv[1]};
	if (!checkFileExists(filename)) {
		std::cerr << "file not exists" << std::endl;
	}

	Core core = Core(filename);
	Code code = Code();

	smatch htmlResult;
	smatch xmlResult;
	if (regex_match(filename, htmlResult, regex(".*\\.html"))) {
		code.setColorMode(HTML_MODE);
	}
	else if (regex_match(filename, xmlResult, regex(".*\\.xml"))) {
		code.setColorMode(HTML_MODE);
	}
	else {
		code.setColorMode(NORMAL_MODE);
	}
	
	code.setText(UTF8toSjis(core.open()));
	code.renderViewCode();
	cout << "\x1b[1;1H";
	SetConsoleTitleA("Text Editor");

	while (true)
	{
		if (isCloseMode) {
			int status = MessageBox(GetConsoleWindow(), TEXT("保存して終了しますか？"), TEXT("Text Editor"), MB_YESNOCANCEL | MB_ICONQUESTION | MB_SYSTEMMODAL);
			switch (status)
			{
			case IDYES:
				systemExit(true, code, core);
			case IDNO:
				systemExit(false, code, core);
			case IDCANCEL:
				isCloseMode = false;
			default:
				break;
			}
		}
		string keyEvent = keyBind();
		SetConsoleTitleA("Text Editor");
		if (keyEvent == "<[ctrl-s]>") {
			vector<string> text = code.getText();
			string content = "";
			for (int i = 0; i < text.size(); i++) {
				content += text[i];
				if (i != text.size() - 1) {
					content += "\n";
				}
			}
			core.save(SjistoUTF8(content));
			SetConsoleTitleA("Saved file !");
		}
		else if (keyEvent == "<[ctrl-q]>") {
			isCloseMode = true;
		}
		else {
			code.renderCode(keyEvent);
		}
	}

	return 0;
}

void systemExit(bool save, Code code, Core core)
{
	if (save) {
		vector<string> text = code.getText();
		string content = "";
		for (int i = 0; i < text.size(); i++) {
			content += text[i];
			if (i != text.size() - 1) {
				content += "\n";
			}
		}
		core.save(SjistoUTF8(content));
		system("cls");
		exit(0);
	}
	else {
		system("cls");
		exit(0);
	}
}
