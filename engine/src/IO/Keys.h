#ifndef KEYS_H
#define KEYS_H

namespace engine {
    namespace IO {
        class Keys {
        public:
            static constexpr char ENTER         = '\x01';
            static constexpr char BACKSPACE     = '\x02';
            static constexpr char ARROW_UP      = '\x03';
            static constexpr char ARROW_LEFT    = '\x04';
            static constexpr char ARROW_RIGHT   = '\x05';
            static constexpr char ARROW_DOWN    = '\x06';
            static constexpr char DEL           = '\x07';
            static constexpr char HOME          = '\x08';
            static constexpr char END           = '\x09';
            static constexpr char CTRL_C        = '\x10';
        };
    }
}

#endif /* KEYS_H */

