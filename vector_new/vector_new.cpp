// vector_new.cpp : Defines the entry point for the application.
//

#include "vector_new.h"
#include <iostream>

int main()
{
	nr::vector<int> vec1{ 1, 2, 3 };
	std::cout << vec1.size() << " " << vec1.capacity() << std::endl;
	for (auto i : vec1) std::cout << i << " ";
	std::cout << std::endl;

	vec1.pop_back();
	vec1.emplace_back();

	std::cout << vec1.size() << " " << vec1.capacity() << std::endl;
	for (auto i : vec1) std::cout << i << " ";
	std::cout << std::endl;

	std::cin.get();
	return 0;
}
