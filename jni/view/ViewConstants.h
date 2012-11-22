#ifndef VIEW_CONSTANTS
#define VIEW_CONSTANTS

namespace game{
    class FloatPosition;

    class ViewConstants {
        public:
        
        static const float PATH_CELL_SIZE_X = 0.07f;
        static const float PATH_CELL_SIZE_Y = 0.064f;
        
        static FloatPosition* LEFT_TOP_CORNER;
    };
}

#endif 