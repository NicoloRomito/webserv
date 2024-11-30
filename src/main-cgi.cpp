#include "../include/includes.hpp"
#include "cgi/Cgi.hpp"
#include "cgi/TRequest.hpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

int main (void)
{
	TRequest request;
	std::vector<std::string> v;
	v.push_back("hi");
	Cgi*		cgi = new Cgi(request, v);

	execve(cgi->getArgv()[0], cgi->getArgv(), cgi->getEnvp());
	std::cout << cgi->getArgv()[0] << "\n";
	std::cout << cgi->getEnvp()[0] << "\n";
}