#include "Strings.h"
#include <ctime>


Strings::Strings()
{
}


Strings::~Strings()
{
}

std::string Strings::currentDateTime()
{
	time_t rawtime = time(nullptr);
	struct tm * dt;
	char buffer[30];
	dt = localtime(&rawtime);
	strftime(buffer, sizeof(buffer), "%m-%d-%H-%M-%y", dt);
	return std::string(buffer);

}