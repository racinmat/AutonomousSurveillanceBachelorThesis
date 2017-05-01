//
// Created by Azathoth on 2017-05-01.
//

#include "FileSystem.h"

#ifdef _WIN32
    #include <Windows.h>
#endif

using namespace std;

namespace App {
    string FileSystem::getCurrentWorkingDir() {
    #ifdef _WIN32
        HMODULE hModule = GetModuleHandleW(NULL);
        WCHAR wcharPath[MAX_PATH];
        GetModuleFileNameW(hModule, wcharPath, MAX_PATH);
        wstring ws(wcharPath);
        std::string path(ws.begin(), ws.end());
        std::string exeName = "SwarmDeployment.exe";
        path = path.substr(0, path.size() - exeName.size());
        return path;
    #elif __linux__
        int MAXPATHLEN = 10000;
            char temp[MAXPATHLEN];
            return ( getcwd(temp, MAXPATHLEN) ? std::string( temp ) : std::string("") );
    #endif
    }

}