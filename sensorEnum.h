//
// Created by theo on 4/23/2021.
//

#ifndef UMDWITHCLASS3_SENSORENUM_H
#define UMDWITHCLASS3_SENSORENUM_H

#define ENUM_SIZE 5


/**
 * SIDs of sensors
 */
enum sensorEnum{
    UNKNOWN_SENSOR = 0,
    MPU9250 = 1,
    ADS1015 = 2,
    ADS1115 = 3,
    INA3221 = 4,
    BMP280 =  5,

    BUILDIN_GPIO = 8,

};

#endif //UMDWITHCLASS3_SENSORENUM_H
