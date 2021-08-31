//
// Created by theo on 8/30/2021.
//

#ifndef UMDWITHCLASS3_GENERALDISPLAY_H
#define UMDWITHCLASS3_GENERALDISPLAY_H


class GeneralDisplay {


    virtual void print(char* text) = 0;
    virtual void print(String text) = 0;
    virtual void println(char* text) { print(text); print('\n')}
    virtual void println(String text) { text += '\n'; print(text)}



};


#endif //UMDWITHCLASS3_GENERALDISPLAY_H
