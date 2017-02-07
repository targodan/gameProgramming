#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl/gl_core_3_3.h"
#include "Bindable.h"
#include "TextureDimension.h"
#include "TextureType.h"
#include "DataType.h"
#include "ImageFormat.h"
#include "TextureParameter.h"
#include "../util.h"
#include "../IOException.h"
#include "../WTFException.h"
#include "../IllegalArgumentException.h"
#include <string>
#include "SOIL/SOIL.h"
#include <memory>

namespace engine {
    namespace renderer {
        using namespace gl;
        
        class Texture : public Bindable {
        public:
            Texture(std::string imagePath, TextureType type);
            Texture(std::string imagePath, ImageFormat format = ImageFormat::RGB, ImageFormat formatToStoreTextureIn = ImageFormat::RGB, 
                    TextureDimension dim = TextureDimension::TEXTURE_2D, TextureType type = TextureType::DIFFUSE);
            Texture(const Texture& orig);
            Texture(Texture&& orig);
            
            Texture& operator=(const Texture& right);
            Texture& operator=(Texture&& right);
            
            virtual ~Texture();
            
            void generateTexture();
            void releaseTexture();
            
            void setParameter(GLenum name, GLenum type);
            
            void loadTexture();
            
            static void activateTextureUnit(GLenum unit);
            
            virtual void bind() override;
            virtual void unbind() override;
            
            virtual bool isBound() const override;
            bool isLoaded() const;
            TextureType getType() const;
            GLuint getID() const;
        private:
            void copyImageData(unsigned char* imageData);
            void initialize(std::string imagePath);
            void generateMipmap();
            
            GLuint id;
            TextureDimension dim;
            TextureType type;
            ImageFormat format;
            ImageFormat formatToStoreTextureIn;
            
            GLsizei width;
            GLsizei height;
            GLsizei depth; // Per default set to one, used for 3D textures (not fully supported))
            
            unsigned char* imageData;
            
            bool bound = false;
            bool loaded = false;
        };
    }
}

#endif /* TEXTURE_H */

