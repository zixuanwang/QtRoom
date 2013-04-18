#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <string>


/// this class defines several constants important for the project.
class GlobalConfig
{
public:
    GlobalConfig();
    const static int THUMBNAIL_WIDTH;
    const static int THUMBNAIL_HEIGHT;
    const static int FULL_WIDTH;
    const static int FULL_HEIGHT;
    const static int CAMERA_IMAGE_BUFFER_SIZE;
    const static int TRAIN_PATCH_WIDTH;
    const static int TRAIN_PATCH_HEIGHT;
    const static std::string ANNOTATE_PATH;
};

#endif // GLOBALCONFIG_H
