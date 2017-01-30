#include "Texture.h"

namespace engine {
    namespace renderer {
        Texture::Texture(std::string imagePath, ImageFormat format, ImageFormat formatToStoreTextureIn, bool specular, TextureType type) 
                    : type(type), format(format), formatToStoreTextureIn(formatToStoreTextureIn), depth(0), bound(false), specular(false) {
//            std::string entireImagePath = imagePath;
            std::string entireImagePath = util::getAbsoluteFromRelativePath(imagePath);
            int forceChannels;
            switch(format) {
                case ImageFormat::RGB:
                    forceChannels = SOIL_LOAD_RGB;
                    break;
                case ImageFormat::RGBA:
                    forceChannels = SOIL_LOAD_RGBA;
                    break;
                default:
                    throw IllegalArgumentException("Only ImageFormats RGB and RGBA are supported yet.");
            }
            this->imageData = SOIL_load_image(entireImagePath.c_str(), &this->width, &this->height, 0, forceChannels);

            if(!imageData) {
                throw engine::IOException("Failed to create texture: Could not load image.");
            }

            this->generateTexture();
        }
        
        Texture::Texture(const Texture& orig) 
            : id(orig.id), type(orig.type), format(orig.format), formatToStoreTextureIn(orig.formatToStoreTextureIn), 
              width(orig.width), height(orig.height), depth(orig.depth), imageData(orig.imageData), bound(orig.bound), specular(orig.specular) {}
        Texture::Texture(Texture&& orig) 
            : id(std::move(orig.id)), type(std::move(orig.type)), format(std::move(orig.format)), formatToStoreTextureIn(std::move(formatToStoreTextureIn)), 
              width(std::move(orig.width)), height(std::move(orig.height)), depth(std::move(orig.depth)), imageData(std::move(orig.imageData)), bound(std::move(bound)), specular(std::move(specular)) {}

        Texture& Texture::operator=(const Texture& right) {
            this->id = right.id;
            this->type = right.type;
            this->format = right.format;
            this->formatToStoreTextureIn = right.formatToStoreTextureIn;
            this->width = right.width;
            this->height = right.height;
            this->depth = right.depth;
            this->imageData = right.imageData;
            this->bound = right.bound;
            this->specular = right.specular;

            return *this;
        }
        Texture& Texture::operator=(Texture&& right){
            this->id = std::move(right.id);
            this->type = std::move(right.type);
            this->format = std::move(right.format);
            this->formatToStoreTextureIn = std::move(right.formatToStoreTextureIn);
            this->width = std::move(right.width);
            this->height = std::move(right.height);
            this->depth = std::move(right.depth);
            this->imageData = std::move(right.imageData);
            this->bound = std::move(right.bound);
            this->specular = std::move(right.specular);

            return *this;
        }

        Texture::~Texture() {}

        void Texture::generateTexture() {
            glGenTextures(1, &(this->id));
        }
        void Texture::releaseTexture() {
            glDeleteTextures(1, &(this->id));
            SOIL_free_image_data(this->imageData);
        }

        void Texture::setParameter(GLenum name, GLenum type) {
#ifdef DEBUG
            if(!this->bound) {
                throw WTFException("Could not set texture parameter: texture not bound.");
            }
#endif

            glTexParameteri(this->type, name, type);
        }

        void Texture::loadTexture() {
#ifdef DEBUG
            if(!this->bound) {
                throw WTFException("Could not load texture: texture not bound.");
            }
#endif

            switch(this->type) {
                case TEXTURE_1D:
                    glTexImage1D(this->type, 0, this->formatToStoreTextureIn, this->width, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                    break;
                case TEXTURE_2D:
                    glTexImage2D(this->type, 0, this->formatToStoreTextureIn, this->width, this->height, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                    break;
                case TEXTURE_3D:
                    glTexImage3D(this->type, 0, this->formatToStoreTextureIn, this->width, this->height, this->depth, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                    break;
                default:
                    throw WTFException("Could not load texture: invalid texture type.");
            }

            this->generateMipmap();
        }

        void Texture::activateTextureUnit(GLenum unit) {
            glActiveTexture(unit);
        }

        void Texture::bind() {
            if(this->bound) {
                return;
            }

            Bindable::bindTexture(this->type, this->id);
            this->bound = true;
        }
        void Texture::unbind() {
            if(!this->bound) {
                return;
            }

            Bindable::unbindTexture(this->type);
            this->bound = false;
        }

        bool Texture::isBound() const {
            return this->bound;
        }
        bool Texture::isSpecular() const {
            return this->specular;
        }
        bool Texture::isDiffuse() const {
            return !this->specular;
        }

        void Texture::generateMipmap() {
#ifdef DEBUG
            if(!this->bound) {
                throw WTFException("Could not generate texture mipmap: texture not bound.");
            }
#endif

            glGenerateMipmap(this->type);
        }
    }
}