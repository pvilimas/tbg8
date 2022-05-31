#include "textbasedgame.hpp"

/* ------- PLAYER ------- */

Player::Player() : currentRoom(Room::InvalidRoom) {}

Room &Player::GetCurrentRoom() {
    return currentRoom;
}

void Player::SetCurrentRoom(Room &r) {
    currentRoom = r;
}

/* ------- TEXTBASEDGAME ------- */

/* static stuff */

/* instance stuff */

TextBasedGame::TextBasedGame() {
    WriteGameOutput = nullptr;
}

TextBasedGame::TextBasedGame(std::function<void(std::string)> _writeFunc) {
    state = State::Title;
    WriteGameOutput = _writeFunc;

    /* init rooms */

    rooms = {
        {"Kitchen", Room("Kitchen")}
    };



    /* init player */

    player.SetCurrentRoom(rooms.at("Kitchen"));

    WriteGameOutput("This is the title msg.");

}

void TextBasedGame::EvalPlayerInput(std::string s) {

    for (Command& c : GetCommands()) {
        if (c.Eval(s)) {
            break;
        }
    }
}

std::vector<Command> TextBasedGame::GetCommands() {
    std::vector<Command> cmds;
    
    /* game commands */
    if (state == State::Title || state == State::Gameplay) {
        cmds.push_back(Command("Help", false, "help", "help( me)?", [&]{ WriteGameOutput("This is the help message."); }));
        cmds.push_back(Command("Quit Game", false, "quit", "(q(uit)?|exit)", [&]{ SetState(TextBasedGame::State::Quitting); }));
    }

    if (state == State::Title) {
        cmds.push_back(Command("Start Game", false, "start", "start( game)?", [&]{ SetState(State::Gameplay); }));
    }

    // quit conf
    if (state == State::Quitting) {
        cmds.push_back(Command("Quit Confirmation: Yes", true, "", "y(es)?", [&]{ throw TextBasedGame::QuitGameException(); }));
        cmds.push_back(Command("Quit Confirmation: No", true, "", "n(o)?", [&]{ SetState(preQuitState); }));
        cmds.push_back(Command("Quit Confirmation: Unknown", true, "", ".*", [&]{ WriteGameOutput("Command not recognized. Are you sure you want to quit? (y/n)s"); }));
    } else {
        cmds.push_back(Command("Failsafe: Match All", true, "", ".*", [&]{ WriteGameOutput("Command not recognized."); }));
    }

    return cmds;
}

void TextBasedGame::SetState(TextBasedGame::State newState) {
    State oldState = state;

    if (oldState == newState) {
        return;
    }

    // if/else stuff here with newState vs oldState

    // if starting the game, write startup message
    if (oldState == State::Title && newState == State::Gameplay) {
        WriteGameOutput("This is the gameplay msg.");
    }
    // if going to quit screen, save previous state to return
    else if (newState == State::Quitting) {
        preQuitState = oldState;
        WriteGameOutput("Are you sure you want to quit? (y/n)");
    }
    // if returning from quit conf
    else if (oldState == State::Quitting) {
        if (newState == State::Title) {
            WriteGameOutput("This is the title screen");
        } else if (newState == State::Gameplay) {
            WriteGameOutput("This is the gameplay msg.");
        }
    }

    state = newState;
}