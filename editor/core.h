#pragma once
#ifndef CORE_H
#define CORE_H

#include <string>

using namespace std;

class Core
{
public:
	Core(string filename);
	void save(string content);
	string open();
private:
	string filename;
};

#endif // !CORE_H