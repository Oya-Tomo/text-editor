#include "core.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Core::Core(string filename)
{
	this->filename = filename;
}

void Core::save(string content)
{
	std::ofstream wf;
	wf.open(this->filename, std::ios::out);
	wf << content;
	wf.close();
}

string Core::open()
{
	std::ifstream rf;
	rf.open(this->filename, std::ios::in);
	return string{ std::istreambuf_iterator<char>(rf), std::istreambuf_iterator<char>() };
}
