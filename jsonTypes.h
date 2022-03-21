//
// Created by theo on 9/27/2021.
//

#ifndef UMDWITHCLASS3_JSONTYPES_H
#define UMDWITHCLASS3_JSONTYPES_H

namespace jsonTypes {
    enum Jsons {
        SET_SENSOR_SETTINGS_JSON = 's',
        GET_SENSOR_SETTINGS_JSON = 'g',

        MANUAL_SENSOR_ADD        = 'a',

        /// clears up one conflict (adds appropriate sensor to sensors ...)
        CLEAR_CONFLICT           = 'C',

        SET_READING_JSON         = 'r',

        GET_SYS_INFO             = 'i',

        CLICK_JSON               = 'c',
        UP_JSON                  = 'u',
        DOWN_JSON                = 'd'
    };
}

#endif //UMDWITHCLASS3_JSONTYPES_H
