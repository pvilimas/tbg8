#ifndef __TBG_HEADER__
#define __TBG_HEADER__

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "command.hpp"
#include "item.hpp"
#include "npc.hpp"
#include "room.hpp"

class Player {

};

class TextBasedGame {

    public:

        struct QuitGameException {};

    private:

        enum State {
            Title,
            Gameplay,
            Quitting,
        };

        struct Messages {
            static std::string
                Title,
                Help,
                PromptQuit;
        };

        State state;
        State preQuitState; // state prior to quit conf - gameplay or title

        Player player;
        // ?
        std::unordered_map<std::string, std::shared_ptr<Room>> rooms;
        std::unordered_map<std::string, std::shared_ptr<Item>> items;
        std::unordered_map<std::string, std::shared_ptr<NPC>> npcs;

    public:
        
        TextBasedGame();
        TextBasedGame(const TextBasedGame&) = delete;
        TextBasedGame(std::function<void(std::string)> _writeFunc);

        /* IO - keep these public */
        void EvalPlayerInput(std::string);
        std::function<void(std::string)> WriteGameOutput;

        // returns a list of all currently valid commands
        std::vector<Command> GetCommands();

        // contains initialization logic for each state
        void SetState(State newState);
    
};

#endif /* __TBG_HEADER__ */
