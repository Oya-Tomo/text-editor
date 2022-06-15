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

using namespace std;

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
	
	code.setText(core.open());
	code.renderViewCode();
	cout << "\x1b[1;1H";

	while (true)
	{
		string keyEvent = keyBind();
		if (keyEvent == "<[ctrl-s]>") {
			vector<string> text = code.getText();
			string content = "";
			for (int i = 0; i < text.size(); i++) {
				content += text[i];
				if (i != text.size() - 1) {
					content += "\n";
				}
			}
			core.save(content);
		}
		else if (keyEvent == "<[ctrl-q]>") {
			vector<string> text = code.getText();
			string content = "";
			for (int i = 0; i < text.size(); i++) {
				content += text[i];
				if (i != text.size() - 1) {
					content += "\n";
				}
			}
			core.save(content);
			system("cls");
			exit(0);
		}
		else {
			code.renderCode(keyEvent);
		}
	}

	return 0;
}