#ifndef TILE_TYPE
#define TILE_TYPE

namespace game{
    class TileType {
        public:
        
        static const int GHOST_HOME 	= 0x01;
        static const int GHOST_PATH 	= 0x02;
        static const int GHOST_TRAP 	= 0x04;
        static const int GHOST_NO_UP	= 0x08;
                     
        static const int PACMAN_PATH    = 0x10;
        static const int TABLET 		= 0x20;
        static const int ENERGIZER 	    = 0x40;
        static const int TUNNEL		    = 0x80;
    };
}

#endif 