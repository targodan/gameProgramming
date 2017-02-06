#include "Texture.h"
#include <easylogging++.h>

namespace engine {
    namespace renderer {
        Texture::Texture(std::string imagePath, TextureType type) 
            : dim(TextureDimension::TEXTURE_2D), type(type), format(ImageFormat::RGB), formatToStoreTextureIn(ImageFormat::RGB), depth(0), bound(false) {
            this->initialize(imagePath);
        }
        Texture::Texture(std::string imagePath, ImageFormat format, ImageFormat formatToStoreTextureIn, TextureDimension dim, TextureType type) 
            : dim(dim), type(type), format(format), formatToStoreTextureIn(formatToStoreTextureIn), depth(0), bound(false) {
            this->initialize(imagePath);
        }
        void Texture::initialize(std::string imagePath) {
            int forceChannels;
            switch(this->format) {
                case ImageFormat::RGB:
                    forceChannels = SOIL_LOAD_RGB;
                    break;
                case ImageFormat::RGBA:
                    forceChannels = SOIL_LOAD_RGBA;
                    break;
                default:
                    throw IllegalArgumentException("Only ImageFormats RGB and RGBA are supported yet.");
            }
            this->imageData = SOIL_load_image(imagePath.c_str(), &this->width, &this->height, 0, forceChannels);

            if(!imageData) {
                throw engine::IOException("Failed to create texture: Could not load image.");
            }

            this->generateTexture();
        }
        
        Texture::Texture(const Texture& orig) 
            : dim(orig.dim), type(orig.type), format(orig.format), formatToStoreTextureIn(orig.formatToStoreTextureIn), 
              width(orig.width), height(orig.height), depth(orig.depth), imageData(orig.imageData), bound(false) {
            this->generateTexture();
            
            if(orig.loaded) {
                this->loadTexture();
            }
        }
        Texture::Texture(Texture&& orig) 
            : id(std::move(orig.id)), dim(std::move(orig.dim)), type(std::move(type)), format(std::move(orig.format)), formatToStoreTextureIn(std::move(formatToStoreTextureIn)), 
              width(std::move(orig.width)), height(std::move(orig.height)), depth(std::move(orig.depth)), imageData(std::move(orig.imageData)), bound(std::move(bound)), loaded(std::move(loaded)) {}

        Texture& Texture::operator=(const Texture& right) {
            this->dim = right.dim;
            this->type = right.type;
            this->format = right.format;
            this->formatToStoreTextureIn = right.formatToStoreTextureIn;
            this->width = right.width;
            this->height = right.height;
            this->depth = right.depth;
            this->imageData = right.imageData;
            this->bound = false;
            this->loaded = false;
            
            this->generateTexture();
            if(right.loaded) {
                this->loadTexture();
            }

            return *this;
        }
        Texture& Texture::operator=(Texture&& right){
            this->id = std::move(right.id);
            this->dim = std::move(right.dim);
            this->type = std::move(right.type);
            this->format = std::move(right.format);
            this->formatToStoreTextureIn = std::move(right.formatToStoreTextureIn);
            this->width = std::move(right.width);
            this->height = std::move(right.height);
            this->depth = std::move(right.depth);
            this->imageData = std::move(right.imageData);
            this->bound = std::move(right.bound);
            this->loaded = std::move(right.loaded);

            return *this;
        }

        Texture::~Texture() {}

        void Texture::generateTexture() {
            glGenTextures(1, &(this->id));
        }
        void Texture::releaseTexture() {
            glDeleteTextures(1, &(this->id));
            // SOIL_free_image_data(this->imageData);
        }

        void Texture::setParameter(GLenum name, GLenum type) {
#ifdef DEBUG
            if(!this->bound) {
                throw WTFException("Could not set texture parameter: texture not bound.");
            }
#endif

            glTexParameteri(this->dim, name, type);
        }

        void Texture::loadTexture() {
#ifdef DEBUG
            if(!this->bound) {
                throw WTFException("Could not load texture: texture not bound.");
            }
#endif
            
            switch(this->dim) {
                case TEXTURE_1D:
                    glTexImage1D(this->dim, 0, this->formatToStoreTextureIn, this->width, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                    break;
                case TEXTURE_2D:
                    glTexImage2D(this->dim, 0, this->formatToStoreTextureIn, this->width, this->height, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                    break;
                case TEXTURE_3D:
                    glTexImage3D(this->dim, 0, this->formatToStoreTextureIn, this->width, this->height, this->depth, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                    break;
                default:
                    throw WTFException("Could not load texture: invalid texture type.");
            }

            this->generateMipmap();
            this->loaded = true;
        }

        void Texture::activateTextureUnit(GLenum unit) {
            glActiveTexture(unit);
        }
        
        TextureType Texture::getType() const {
            return this->type;
        }

        void Texture::bind() {
            if(this->bound) {
                return;
            }

            Bindable::bindTexture(this->dim, this->id);
            this->bound = true;
        }
        void Texture::unbind() {
            if(!this->bound) {
                return;
            }

            Bindable::unbindTexture(this->dim);
            this->bound = false;
        }
        
        bool Texture::isLoaded() const {
            return this->loaded;
        }
        bool Texture::isBound() const {
            return this->bound;
        }

        void Texture::generateMipmap() {
#ifdef DEBUG
            if(!this->bound) {
                throw WTFException("Could not generate texture mipmap: texture not bound.");
            }
#endif

            glGenerateMipmap(this->dim);
        }
    }
}