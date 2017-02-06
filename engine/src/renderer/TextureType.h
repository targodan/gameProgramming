#ifndef TEXTURETYPE_H
#define TEXTURETYPE_H

namespace engine {
    namespace renderer {
        enum TextureType {
            DIFFUSE = 1,
            SPECULAR = 2,
            NORMAL = 4,
            HEIGHT = 8
        };
    }
}

#endif /* TEXTURETYPE_H */

