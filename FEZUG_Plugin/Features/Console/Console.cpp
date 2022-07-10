#include "Console.hpp"

#include "Modules/Overlay.hpp"
#include "Modules/Input.hpp"
#include "Utils/Math.hpp"
#include "Utils/Logger.hpp"
#include "Command.hpp"

#include <algorithm>

Console* gConsole = new Console();

void Console::Update() {

    const UINT conKey = VK_OEM_3;

    if (gInput->IsKeyPressed(conKey)) {
        Toggle();
        gInput->SetGameInputEnabled(!IsEnabled());
    }

    positionInterp += ((IsEnabled() ? 1.0f : 0.0f) - positionInterp) * 0.2f;
    if (positionInterp < 0.01f)positionInterp = 0.0f;

    if (IsEnabled()) {
        // making sure typeBuffer contains pairs of %% to not fuck with printf formatting
        string typeBuffer = Utils::StringReplace(gInput->GetTypeBuffer(), "%", "%%");

        if (typeBuffer.size() > 0 && !gInput->IsKeyPressed(conKey)) {
            if (typeBuffer.find('\b') != string::npos) {
                typingField = typingField.substr(0, typingField.size() - 1);
            }
            else {
                typingField += typeBuffer;
            }
            
        }

        if (gInput->IsKeyPressed(VK_RETURN)) {
            ProcessTypingField();
        }
    }
    
}

void Console::ProcessTypingField() {
    WriteLine("] %s", typingField.c_str());

    // the shitcode for splitting the command

    string command;
    vector<string> params;
    
    int lastCut = 0;
    bool skippedFirst = false;
    bool isQuoted = false;

    typingField += " ";
    int typeSize = typingField.size();
    for (int i = 0; i < typeSize; i++) {
        char c = typingField[i];
        if (
            (c == ' ' && (!isQuoted || i+1 == typeSize)) ||
            (c == '"' && skippedFirst)
        ) {
            int len = i - lastCut;
            if (len > 0) {
                string cut = typingField.substr(lastCut, len);
                if (!skippedFirst) {
                    command = cut;
                    skippedFirst = true;
                }
                else {
                    params.push_back(cut);
                }
            }
            lastCut=i+1;
        }
        if (c == '"' && skippedFirst) {
            isQuoted = !isQuoted;
        }
    }

    //quick test if the shitcode above works
    cout << "[Console test] " << command << ": ";
    for (const string& param : params) {
        cout << param << "|";
    }
    cout << endl;

    if (command.size() > 0) {
        Command* foundCmd = nullptr;
        for (Command* c : Command::GetList()) {
            if (command.compare(c->GetName()) == 0) {
                foundCmd = c;
                break;
            }
        }
        if (foundCmd) {
            if (foundCmd->IsParamCountValid(params.size())) {
                foundCmd->Execute(params);
            }
            else {
                WriteLine("Incorrect usage of command: %s", command.c_str());
            }
            
        }
        else {
            WriteLine("Unknown command: %s", command.c_str());
        }
    }
    

    typingField.clear();
}


void Console::Clear() {
    logField.clear();
}

void Console::WriteLine(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1023, format, args);

    string sOutput = string(buffer);
    Logger::Log("FEZUG Console", sOutput);

    logField.push_back({ sOutput });
    if (logField.size() > 9) {
        logField.pop_front();
    }
}


void Console::Draw() {
    
    if (!IsEnabled() && positionInterp==0) return;

    Vector2 wSize = gOverlay->GetWindowSize();

    int lineHeight = 25;
    int lineCount = 9;
    int hPadding = 20;
    int vPadding = 15;
    int midGap = 4;

    int uiHeight = lineHeight * lineCount + lineHeight + vPadding * 3 + midGap*4;
    int ypos = -uiHeight * (1.0f-positionInterp);

    glTranslatef(0, ypos, 0);

    const Overlay::Color uiColorMain = Overlay::Color(128, 86, 214);
    const Overlay::Color uiColorDarker = Overlay::Color(77, 36, 140, 240);
    const Overlay::Color uiColorBorderLight = Overlay::Color(255, 255, 255, 128);
    const Overlay::Color uiColorBorderBlack = Overlay::Color(0, 0, 0, 128);

    // main background
    gOverlay->FillRectangle(0, 0, wSize.x, uiHeight, uiColorDarker);

    // highlight lines
    gOverlay->DrawLine(hPadding + 1, 0, hPadding + 1, uiHeight, uiColorBorderBlack);
    gOverlay->DrawLine(wSize.x - hPadding - 1, 0, wSize.x - hPadding - 1, uiHeight, uiColorBorderLight);
    int lineHeights[4] = { 
        vPadding + 1, 
        uiHeight - vPadding * 2 - lineHeight - midGap * 2 - 1,
        uiHeight - vPadding - lineHeight - midGap * 2 + 1,
        uiHeight - vPadding - 1
    };
    for(int i=0;i<4;i++) gOverlay->DrawLine(
        hPadding + 1, lineHeights[i], wSize.x - hPadding - 1, lineHeights[i],
        (i%2==0) ? uiColorBorderBlack : uiColorBorderLight
    );

    // solid columns
    gOverlay->FillRectangle(0, 0, hPadding, uiHeight, uiColorMain);
    gOverlay->FillRectangle(wSize.x - hPadding, 0, hPadding, uiHeight, uiColorMain);

    // solid sector separators
    gOverlay->FillRectangle(hPadding, 0, wSize.x - hPadding * 2, vPadding, uiColorMain);
    gOverlay->FillRectangle(hPadding, uiHeight - vPadding*2 - lineHeight - midGap*2, wSize.x - hPadding * 2, vPadding, uiColorMain);
    gOverlay->FillRectangle(hPadding, uiHeight - vPadding, wSize.x - hPadding * 2, vPadding, uiColorMain);

    //drawing text
    int textPos = vPadding;
    for (auto const &log : logField) {
        textPos += lineHeight;
        gOverlay->DrawFontText(1, hPadding + midGap + 2, textPos, log.color, log.text.c_str());
    }
    
    gOverlay->DrawFontText(1, hPadding + midGap + 2, uiHeight - vPadding - midGap*2, Overlay::Color(255, 255, 255), typingField.c_str());

    glTranslatef(0, -ypos, 0);
}



// console related commands

CREATE_COMMAND_PARAMS(echo, "Prints text to console.", -1) {
    string s;
    for (const string& param : params) {
        s += param + " ";
    }
    gConsole->WriteLine("%s", s.c_str());
}

CREATE_COMMAND_SIMPLE(clear, "Clears the console window.") {
    gConsole->Clear();
}