//
// Created by theo on 9/20/2021.
//

#include "Setting.h"


void Setting::onUp() {
    if (currentIndex != 0) currentIndex--;
}

void Setting::onDown(){
    if (currentIndex + 1 < values.size()) currentIndex++;
}