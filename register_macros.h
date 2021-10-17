//
// Created by theo on 10/17/2021.
//

#ifndef UMDWITHCLASS3_REGISTER_MACROS_H
#define UMDWITHCLASS3_REGISTER_MACROS_H

#include <cstdint>

#define IO_MUX_GPIO_21_REG 0x3FF4907C
#define IO_MUX_GPIO_22_REG 0x3FF49080

void set_io_mux_gpio(uint32_t* reg, uint32_t def);

#endif //UMDWITHCLASS3_REGISTER_MACROS_H
