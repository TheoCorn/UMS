//
// Created by theo on 9/14/2021.
//

#ifndef UMDWITHCLASS3_SYSINFOTAB_H
#define UMDWITHCLASS3_SYSINFOTAB_H

#include "ui.h"
#include "sysInfo.h"


class SysInfoTab : public ui::Tab{
    String name() override { return "SysInfo"; };

    void render(Adafruit_SSD1306 *display, ui::coordinates &start, ui::coordinates &end) override;

};


#endif //UMDWITHCLASS3_SYSINFOTAB_H
