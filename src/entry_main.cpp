/**
 * @file entry_main.cpp
 * @author Yahor Senichak (xsenic00)
 * @brief Entry point of the programm 
 */

#include <app.h>




/// @brief Entry poinjt of the programm 
/// @param argc [unused]
/// @param argv [unused]
/// @return finish code 
int main(int argc, char **argv)
{
	return App{argc, argv}.start();
}
