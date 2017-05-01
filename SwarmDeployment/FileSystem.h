//
// Created by Azathoth on 2017-05-01.
//

#ifndef SWARMDEPLOYMENT_FILESYSTEM_H
#define SWARMDEPLOYMENT_FILESYSTEM_H

#include <string>

using namespace std;

namespace App
{

    class FileSystem {
    public:
        static string getCurrentWorkingDir();
    };
}
#endif //SWARMDEPLOYMENT_FILESYSTEM_H
