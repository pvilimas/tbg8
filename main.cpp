#include "game.hpp"

/*
    TODO:

    - rooms
    - invent a couple (kitchen bedroom garden)
    - player.current room
    - moving

    GUIDELINES:

    - do not use 'using namespace x' anywhere

    NOTES:

    - game rooms/items/npcs are stored in map<string, _>s
    - DO NOT USE POINTERS
    - player.currentroom and room.paths should use references into those maps
*/

int main() {
    SetTraceLogLevel(LOG_WARNING);
    ChangeDirectory(GetApplicationDirectory());

    Game *g = new Game();
    g->Init();
    try {
        g->Run();
    } catch (TextBasedGame::QuitGameException& q) {}
    delete g;

    return 0;
}