#include "textbox.hpp"

TextBox::TextBox(std::function<void(std::string)> _onEnter) {
    rec = Rectangle { 0, 360, 640, 100 };
    cursorPos = 0;
    onEnter = _onEnter;

    purgeQueue = false;
}

void TextBox::PollKeyInput() {

    if (!textQueue.empty()) {
        if (GetKeyPressed() != 0) {
            purgeQueue = true;
        }
        return;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (textIn.length() > 0) {
            onEnter(textIn);
            textIn.clear();
            cursorPos = 0;
        }
    } else if (IsKeyPressed(KEY_LEFT)) {
        cursorPos--;
        if (cursorPos < 0) {
            cursorPos = 0;
        }
    } else if (IsKeyPressed(KEY_RIGHT)) {
        cursorPos++;
        if (cursorPos > (PlayerPrompt + textIn).length()-1) {
            cursorPos = (PlayerPrompt + textIn).length()-1;
        }
    } else if (IsKeyPressed(KEY_UP)) {
        cursorPos = 0;
    } else if (IsKeyPressed(KEY_DOWN)) {
        cursorPos = (PlayerPrompt + textIn).length()-1;
    }

    // Get char pressed (unicode character) on the queue
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0) {
        // NOTE: Only allow keys in range [32..125]
        // TODO: customize range later (KEY_GRAVE and others)
        if ((key >= 32) && (key <= 125)) {
            textIn.insert(textIn.begin() + cursorPos, (char)key);
            cursorPos++;
        }

        key = GetCharPressed();  // Check next character in the queue
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (textIn.length() > 0 && cursorPos > 0) {
            textIn.erase(cursorPos-1, 1);
            cursorPos--;
        }
    }
}

void TextBox::Draw() {
    // which line text is being added to
    static int _currLine = 0;
    static char c;

    PollKeyInput();

    /* update text queue */

    if (!textQueue.empty()) {
        do {
            c = textQueue.front();
            if (c == '\n') {
                _currLine++;
            } else {
                textOut[_currLine] += c;
            }
            textQueue.pop();
        } while (!textQueue.empty() && purgeQueue);
        purgeQueue = false;
    } else {
        _currLine = 0;
    }

    /* textbox */
    DrawRectangleRec(rec, Color {5, 5, 5, 255});

    /* text */
    DrawText(textOut[0].c_str(), rec.x+2, rec.y, 20, Color {180, 180, 180, 255});
    DrawText(textOut[1].c_str(), rec.x+2, rec.y+25, 20, Color {180, 180, 180, 255});
    DrawText(textOut[2].c_str(), rec.x+2, rec.y+50, 20, Color {180, 180, 180, 255});
    DrawText((PlayerPrompt + textIn).c_str(), rec.x, rec.y+75, FontSize, WHITE);

    /* cursor */

    float cursor_x = rec.x + 2 + MeasureText((PlayerPrompt + textIn.substr(0, cursorPos)).c_str(), FontSize);
    float cursor_y = rec.y+75;
    float cursor_w = 2;
    float cursor_h = FontSize;

    /* cursor blinking */
    if ((int)(GetTime() * 10000) % 15000 >= 7500) {
        DrawLineEx({ cursor_x, cursor_y }, { cursor_x, cursor_y + cursor_h }, cursor_w, WHITE);
    }

}

void TextBox::Write(std::string s) {

    for (std::string& line : textOut) {
        line.clear();
    }

    /* split text into lines */
    static std::regex line_regex = std::regex("(.{1,58})(?:(\\s)+|$)");

    std::queue<std::string> res;
    int lc = 0;
    for (std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), line_regex); i != std::sregex_iterator(); i++) {
        if (lc == TextBox::LineCount) {
            break;
        }
        res.push((*i).str() + "\n");
        lc++;
    }

    // pad it with empty lines
    while (0 && res.size() < TextBox::LineCount) {
        res.push("");
    }

    // clear queue
    textQueue = std::queue<char>();

    while (!res.empty()) {
        auto line = res.front();
        for (char c : line) {
            textQueue.push(c);
        }
        res.pop();
    }
}
