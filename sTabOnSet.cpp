//
// Created by theo on 9/21/2021.
//

#include "sTabOnSet.h"


void sTabOnSet::comChange(unsigned int index){
    delay(1000);
    delete sysInfo::serialCom;

    sysInfo::serialCom = getSerialCom4EnumPos(index);
}
