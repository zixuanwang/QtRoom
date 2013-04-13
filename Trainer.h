#ifndef TRAINER_H
#define TRAINER_H

#include "HoGDescriptor.h"
#include "SVMClassifier.h"
#include "Utility.h"

#define PATCH_WIDTH 64
#define PATCH_HEIGHT 64

class Trainer
{
public:
    Trainer();
    static void train_hog(const std::string& train_directory);
    static void train_cascade(const std::string& train_directory);
};

#endif // TRAINER_H
