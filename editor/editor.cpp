#include <iostream>
#include <numeric>
#include "code.h"
#include "key.h"
#include "utils.h"

using namespace std;

int main()
{
	Code code = Code();
	
	while (true)
	{
		code.renderCode(keyBind());
	}

	return 0;
}