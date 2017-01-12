/* 
 * File:   Device.h
 * Author: markus
 *
 * Created on 29. Dezember 2016, 14:53
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <string>
namespace engine {
    namespace IO {
        typedef struct Device {
            int deviceID;
            std::string name;
        } device;
    }
}

#endif /* DEVICE_H */

