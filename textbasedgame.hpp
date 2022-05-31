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

    private:

        Room &currentRoom;

    public:

        Player();
        Player(const Player&) = delete;

        Room &GetCurrentRoom();
        void SetCurrentRoom(Room &r);
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

        // todo add consts here
        struct Messages {
            static inline std::string
                Title = "You are on the title screen.",
                Help = "This is the help message.",
                PromptQuit = "Do you want to quit? (y/n)";

                // TODO: functions here, to copy the exact verb the player used, unless it's invalid, in which case "what do you want to do?"
                //                                   ^ eg. "what do you want to pick up?"
                // maybe look at STR_KEYWORDS from tbg5/alpha/globals to get an idea
        };

        State state;
        State preQuitState; // state prior to quit conf - gameplay or title

        Player player;
        std::unordered_map<std::string, Room> rooms;
        std::unordered_map<std::string, Item> items;
        std::unordered_map<std::string, NPC> npcs;

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
