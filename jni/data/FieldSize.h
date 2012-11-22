#ifndef FIELD_SIZE
#define FIELD_SIZE

namespace game{
    class FieldSize {
        public:
        
        static const int WIDTH 	= 30;
        static const int HEIGHT = 31;
    };
    
    typedef int PathArray[FieldSize::HEIGHT][FieldSize::WIDTH];
}

#endif 