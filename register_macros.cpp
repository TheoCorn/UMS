//
// Created by theo on 10/17/2021.
//

#include "register_macros.h"

void set_io_mux_gpio(uint32_t* reg, uint32_t def){
    *reg = *reg >> 16;
    *reg = *reg << 16;

    *reg |= def;
}

