#ifndef SHADERTYPE_H

#define SHADERTYPE_H



#include "gl/gl_core_3_3.h"



#include <functional>

#include <type_traits>



namespace engine {

    namespace renderer {

        enum ShaderType {

            NO_SHADER = -1,

            VERTEX_SHADER = GL_VERTEX_SHADER,

            FRAGMENT_SHADER = GL_FRAGMENT_SHADER,

            GEOMETRY_SHADER = GL_GEOMETRY_SHADER

        };

    }

}



namespace std {

    template<>

    struct hash<engine::renderer::ShaderType> {

       typedef engine::renderer::ShaderType argument_type;

       typedef size_t result_type;



       result_type operator() (const argument_type& x) const

       {

          using type = typename std::underlying_type<argument_type>::type;

          return std::hash<type>()(static_cast<type>(x));

       }

    };

}



#endif /* SHADERTYPE_H */

