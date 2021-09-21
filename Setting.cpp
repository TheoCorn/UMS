//
// Created by theo on 9/20/2021.
//

#include "Setting.h"


void GeneralSetting::onUp() {
    if (currentIndex != 0) currentIndex--;
}

void GeneralSetting::onDown(){
    if (currentIndex + 1 < values.size()) currentIndex++;
}