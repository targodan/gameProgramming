/* 
 * File:   ButtonMapping.cpp
 * Author: markus
 * 
 * Created on 27. Dezember 2016, 14:13
 */

#include "ButtonMapping.h"
namespace engine {
    namespace IO {
        ButtonMapping::ButtonMapping(MessageHandler handler) : handler(handler) {
            this->Mapping.set_empty_key(SIZE_MAX);
        }

        ButtonMapping::~ButtonMapping() {
        }
        
        ButtonMapping::insertMapping(int buttonId, Message msg){
            if(this->Mapping[buttonId] == nullptr) {
                this->Mapping[buttonId] = msg;
            }
        }
        
        ButtonMapping::deleteMapping(int buttonId) {
            this->Mapping.erase(buttonId);
        }
        
        ButtonMapping::deleteMapping(Message msg) {
            for(auto it = this->Mapping.begin(); it != this->Mapping.end(); it++) {
                if(it.pos->second == msg){
                    this->Mapping.erase(it);
                }
            }
        }
        
        ButtonMapping::dispatchMessages() {
            
        }
    }
}
