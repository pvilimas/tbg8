#include "room.hpp"

Room Room::InvalidRoom = Room("Invalid");

Room::Room(std::string _name) {
    name = _name;
}

std::string &Room::GetName() {
    return name;
}
