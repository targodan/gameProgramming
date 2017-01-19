/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ButtonInfo.h
 * Author: markus
 *
 * Created on 17. Januar 2017, 21:29
 */

#ifndef BUTTONINFO_H
#define BUTTONINFO_H
namespace engine {
    namespace IO {
        struct ButtonInfo{
            int buttonID, deviceID;
            float xAxis, yAxis;
        };
    }
}


#endif /* BUTTONINFO_H */

