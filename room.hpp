#ifndef __ROOM_HEADER__
#define __ROOM_HEADER__

#include <string>

#include "item.hpp"

class Room {
    private:

        std::string name;

    public:

        static Room InvalidRoom;

        Room(std::string _name);

        std::string &GetName();
};

#endif /* __ROOM_HEADER__ */