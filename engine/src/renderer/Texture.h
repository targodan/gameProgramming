#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl/gl_core_3_3.h"
#include "Bindable.h"
#include "TextureType.h"
#include "DataType.h"
#include "ImageFormat.h"
#include "TextureParameter.h"
#include "../util.h"
#include "../IOException.h"
#include "../WTFException.h"
#include <string>
#include "SOIL/SOIL.h"
#include <memory>

namespace engine {
    namespace renderer {
        using namespace gl;
        
        class Texture : public Bindable {
        public:
            Texture(std::string imagePath, ImageFormat format = ImageFormat::RGB, bool specular = false, TextureType type = TextureType::TEXTURE_2D) : type(type), format(format), depth(0), bound(false), specular(false) {
                std::string entireImagePath = util::getAbsoluteFromRelativePath(imagePath);
                this->imageData = SOIL_load_image(entireImagePath.c_str(), &this->width, &this->height, 0, SOIL_LOAD_RGB);
                
                if(!imageData) {
                    throw engine::IOException("Failed to create texture: Could not load image.");
                }
                
                this->generateTexture();
            };
            Texture(const Texture& orig) : id(orig.id), type(orig.type), format(orig.format), width(orig.width), height(orig.height), depth(orig.depth), imageData(orig.imageData), bound(orig.bound), specular(orig.specular) {};
            Texture(Texture&& orig) : id(std::move(orig.id)), type(std::move(orig.type)), format(std::move(orig.format)), width(std::move(orig.width)), height(std::move(orig.height)), depth(std::move(orig.depth)), 
                                      imageData(std::move(orig.imageData)), bound(std::move(bound)), specular(std::move(specular)) {};
            
            Texture& operator=(const Texture& right) {
                this->id = right.id;
                this->type = right.type;
                this->format = right.format;
                this->width = right.width;
                this->height = right.height;
                this->depth = right.depth;
                this->imageData = right.imageData;
                this->bound = right.bound;
                this->specular = right.specular;
                
                return *this;
            }
            Texture& operator=(Texture&& right){
                this->id = std::move(right.id);
                this->type = std::move(right.type);
                this->format = std::move(right.format);
                this->width = std::move(right.width);
                this->height = std::move(right.height);
                this->depth = std::move(right.depth);
                this->imageData = std::move(right.imageData);
                this->bound = std::move(right.bound);
                this->specular = std::move(right.specular);
                
                return *this;
            }
            
            virtual ~Texture() {
            };
            
            void generateTexture() {
                glGenTextures(1, &(this->id));
            }
            void releaseTexture() {
                glDeleteTextures(1, &(this->id));
                SOIL_free_image_data(this->imageData);
            }
            
            void setParameter(GLenum name, GLenum type) {
#ifdef DEBUG
                if(!this->bound) {
                    throw WTFException("Could not set texture parameter: texture not bound.");
                }
#endif
                
                glTexParameteri(this->type, name, type);
            }
            
            void loadTexture(ImageFormat formatToStoreTextureIn = ImageFormat::RGB) {
#ifdef DEBUG
                if(!this->bound) {
                    throw WTFException("Could not load texture: texture not bound.");
                }
#endif
                
                switch(this->type) {
                    case TEXTURE_1D:
                        glTexImage1D(this->type, 0, formatToStoreTextureIn, this->width, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                        break;
                    case TEXTURE_2D:
                        glTexImage2D(this->type, 0, formatToStoreTextureIn, this->width, this->height, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                        break;
                    case TEXTURE_3D:
                        glTexImage3D(this->type, 0, formatToStoreTextureIn, this->width, this->height, this->depth, 0, this->format, DataType::UBYTE, (const GLvoid*) this->imageData);
                        break;
                    default:
                        throw WTFException("Could not load texture: invalid texture type.");
                }
                
                this->generateMipmap();
            }
            
            static void activateTextureUnit(GLenum unit) {
                glActiveTexture(unit);
            }
            
            virtual void bind() override {
                if(this->bound) {
                    return;
                }
                
                Bindable::bindTexture(this->type, this->id);
                this->bound = true;
            }
            virtual void unbind() override {
                if(!this->bound) {
                    return;
                }
                
                Bindable::unbindTexture(this->type);
                this->bound = false;
            }
            
            virtual bool isBound() const override {
                return this->bound;
            }
            bool isSpecular() const {
                return this->specular;
            }
            bool isDiffuse() const {
                return !this->specular;
            }
        private:
            void generateMipmap() {
#ifdef DEBUG
                if(!this->bound) {
                    throw WTFException("Could not generate texture mipmap: texture not bound.");
                }
#endif
                
                glGenerateMipmap(this->type);
            }
            
            GLuint id;
            TextureType type;
            ImageFormat format;
            
            GLsizei width;
            GLsizei height;
            GLsizei depth; // Per default set to zero, used for 3D textures (not fully supported))
            
            unsigned char* imageData;
            
            bool bound;
            bool specular;
        };
    }
}

#endif /* TEXTURE_H */

