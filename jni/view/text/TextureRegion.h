#ifndef TEXTURE_REGION
#define TEXTURE_REGION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace game{
    class TextureRegion {
        public:
        
        // Top/Left U,V Coordinates
        float u1;
        float v1;
        
        // Bottom/Right U,V Coordinates
        float u2;
        float v2;
        
        //--Constructor--//
        // D: calculate U,V coordinates from specified texture coordinates
        // A: texWidth, texHeight - the width and height of the texture the region is for
        //    x, y - the top/left (x,y) of the region on the texture (in pixels)
        //    width, height - the width and height of the region on the texture (in pixels)
        TextureRegion(float texWidth, float texHeight, float x, float y, float width, float height);
    };
}

#endif 