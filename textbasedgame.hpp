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

        Room *currentRoom;

        std::vector<std::shared_ptr<Item>> inventory;

        // auxmethod for repr inv 
        inline std::string fullRepr(std::string itemRepr);
    
    public:

        Player();

        Room *GetCurrentRoom();
        void SetCurrentRoom(Room* r);

        std::vector<std::shared_ptr<Item>> GetInventory();
        std::string ReprInventory();

        bool HasItem(std::shared_ptr<Item> i);
        void TakeItem(std::shared_ptr<Item> i);
        void DropItem(std::shared_ptr<Item> i);

        void Move(Direction dir);
};

class TextBasedGame {

    public:

        struct QuitGameException {};

    private:

        struct DirectionSet {
            Direction North, East, South, West;
        };

        enum State {
            Title,
            Gameplay,
            Quitting,
        };

        struct Messages {
            static std::string
                Title,
                Help,
                PromptQuit,
                ErrorBlockedDir,
                ErrorInvalidDir,
                ErrorMissingDir,
                ErrorInvalidTake,
                ErrorInvalidTakeMissing,
                ErrorIllegalTake,
                ErrorIllegalUse,
                ErrorIllegalUseThatWay,
                ErrorInvalidUse,
                ErrorInvalidDrop,
                ErrorMissingTake,
                ErrorMissingUse,
                ErrorMissingDrop,
                ErrorUnknownItem,
                ErrorUnknownCmd,
                ErrorUnknownQuitCmd;
        };

        State state;
        State preQuitState; // state prior to quit conf - gameplay or title

        Player player;
        std::unordered_map<std::string, std::shared_ptr<Room>> rooms;
        std::unordered_map<std::string, std::shared_ptr<Item>> items;
        std::unordered_map<std::string, std::shared_ptr<NPC>> npcs;

    public:

        static DirectionSet Directions;
        
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

        // contains failure check
        void TryMove(Direction dir);

        // contains failure check
        void TryTakeItem(std::shared_ptr<Item> i);
        // contains failure check
        void TryDropItem(std::shared_ptr<Item> i);
    
};

#endif /* __TBG_HEADER__ */
