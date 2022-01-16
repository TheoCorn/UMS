//
// Created by theo on 5/10/2021.
//

#ifndef UMDWITHCLASS3_ASCIIMAKROS_H
#define UMDWITHCLASS3_ASCIIMAKROS_H
//end of text
#define ETX 3
#define DC1_SLEEP 21
#define DC2_DISPLAY_TOGGLE 22
//start of text
#define STX 2
//acknowledge
#define ACK 6

/**
 * ascii values
 *
 */
namespace av{
    constexpr uint8_t etx = 3;
    constexpr uint8_t stx = 2;
    constexpr uint8_t ack = 6;
}

#endif //UMDWITHCLASS3_ASCIIMAKROS_H
