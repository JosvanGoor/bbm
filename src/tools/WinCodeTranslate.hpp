#ifndef TOOLS_WINCODETRANSLATE
#define TOOLS_WINCODETRANSLATE

#include <string>
#include <windows.h>

#define T_CALLBACK 1
#define T_MESSAGE 2
#define T_KEYCODE 3

std::string translate(int message, int type);

std::string vkCodeToString(int key);
std::string messageToString(size_t msg);



#endif // TOOLS_WINMESSAGE

