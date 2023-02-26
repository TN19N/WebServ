// -------------------------------------------------------------------------------
//   Created by Jaouad Chakir (jchakir@student.1337.ma) on 2/21/23 at 5:41 PM.
//   Copyright (c) 2023  1337.ma(@1337FIL) . All rights reserved.
// -------------------------------------------------------------------------------

#include "request/request.hpp"

int main()
{
	std::ifstream file;
	
	file.open("./core");
	if (file.good())
		std::cout << "is file\n";
	else
		std::cout << "is directory\n";
}
