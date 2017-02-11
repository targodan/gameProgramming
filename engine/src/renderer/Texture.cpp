#include "Texture.h"
#include <easylogging++.h>

namespace engine {
    namespace renderer {
        Texture::Texture(std::string imagePath, TextureType type) 
            : dim(TextureDimension::TEXTURE_2D), type(type), format(ImageFormat::RGB), formatToStoreTextureIn(ImageFormat::RGB), depth(1) {
            this->initialize(imagePath);
        }
        Texture::Texture(std::string imagePath, ImageFormat format, ImageFormat formatToStoreTextureIn, TextureDimension dim, TextureType type) 
            : dim(dim), type(type), format(format), formatToStoreTextureIn(formatToStoreTextureIn), depth(1) {
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
              width(orig.width), height(orig.height), depth(orig.depth) {
            this->copyImageData(orig.imageData);
            this->generateTexture();
            if(orig.loaded) {
                this->bind();
                this->loadTexture();
                this->unbind();
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
            this->bound = false;
            this->loaded = false;
            
            this->copyImageData(right.imageData);
            this->generateTexture();
            if(right.loaded) {
                this->bind();
                this->loadTexture();
                this->unbind();
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
            
            LOG(INFO) << "Created texture: " << std::to_string(this->id);
        }
        void Texture::releaseTexture() {
            glDeleteTextures(1, &(this->id));
            // SOIL_free_image_data(this->imageData);
            LOG(INFO) << "Released texture: " << std::to_string(this->id);
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
            
            LOG(INFO) << "Loaded texture: " << std::to_string(this->id);
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
        GLuint Texture::getID() const {
            return this->id;
        }

        void Texture::generateMipmap() {
#ifdef DEBUG
            if(!this->bound) {
                throw WTFException("Could not generate texture mipmap: texture not bound.");
            }
#endif

            glGenerateMipmap(this->dim);
        }
        
        void Texture::copyImageData(unsigned char* imageData) {
            unsigned int nChannels = 0;
            switch(this->format) {
                case ImageFormat::RGB:
                    nChannels = 3;
                    break;
                case ImageFormat::RGBA:
                    nChannels = 4;
                    break;
                default:
                    throw IllegalArgumentException("Only ImageFormats RGB and RGBA are supported yet.");
            }
            
            unsigned int bufferSize = this->width*this->height*this->depth*nChannels;
            this->imageData = new unsigned char[bufferSize];
            std::copy(imageData, imageData + bufferSize, this->imageData);
        }
    }
}