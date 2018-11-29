#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include "game.h"
using namespace std;

// Useful constants
const int DEFAULT_MULTIPLIER = 1;

// probably move this later
void addCommands(vector<string> &commands) {
 commands.emplace_back("left");
 commands.emplace_back("right");
 commands.emplace_back("down");
 commands.emplace_back("clockwise");
 commands.emplace_back("counterclockwise");
 commands.emplace_back("drop");
 commands.emplace_back("levelup");
 commands.emplace_back("leveldown");
 commands.emplace_back("norandom");
 commands.emplace_back("random");
 commands.emplace_back("sequence");
 commands.emplace_back("I");
 commands.emplace_back("J");
 commands.emplace_back("K");
 commands.emplace_back("L");
 commands.emplace_back("S");
 commands.emplace_back("Z");
 commands.emplace_back("T");
 commands.emplace_back("O");
 commands.emplace_back("restart");
}

void addToDictionary(map<string, string> &dict, string command, vector<string> &cmds) {
 smatch match;
 for (int i = 1; i <= (int) command.length(); ++i) {
  string key = command.substr(0, i);
  regex subStr{"^" + key};
  int matches = 0;
  for (auto &word : cmds) { // check that the key maps to only one command
   if (regex_search(word, match, subStr)) ++matches;
   }
   if (matches == 1) dict[key] = command;
 }
}

void initializeDict(map<string, string> &myDict, vector<string> &commands) {
 for (string &cmd : commands) {
  addToDictionary(myDict, cmd, commands);
  }
}

int getNumPrefix(string &command) { 
 int n;
 istringstream iss {command};
 if (iss >> n) { // the command did begin with a number
  ostringstream oss;
  oss << n;
  command = command.substr(oss.str().length()); // strip the number from the front
  return n;
 } else {
  // command unchanged
  return DEFAULT_MULTIPLIER;
 }
} 

bool validSpecialAction(string action) {
 if (action == "force") return true;
 if (action == "heavy") return true;
 if (action == "blind") return true;
 return false;
}

bool validBlock(char c) {
 switch (c) {
  case 'S' : return true;
  case 's' : return true;
  case 'Z' : return true;
  case 'z' : return true;
  case 'I' : return true;
  case 'i' : return true;
  case 'J' : return true;
  case 'j' : return true;
  case 'L' : return true;
  case 'l' : return true;
  case 'O' : return true;
  case 'o' : return true;
  case 'T' : return true;
  case 't' : return true;
  default : return false;
 }
}

int main () {
    srand(time(NULL));
    vector<string> commands;
    addCommands(commands);
    map<string, string> cmdMappings;
    initializeDict(cmdMappings, commands);

    string command;
    unique_ptr<Game>game{new Game};
    game->print();
 // Command interpreter
     while (!game->gameOver() && !cin.eof()) {
      
      if (game->isNeedSpecial()) {// needs error checking
       string action;
       cout << "Please enter a special action" << endl;
       
        while (true) {
         cin >> action;
         if (cin.fail()) {
          cin.clear();
          cin.ignore();
          continue;
          }
         
         if (validSpecialAction(action)) break;
         cout << "Invalid action. Please enter 'blind', 'heavy', or 'force.'" << endl;
         }

       if (action == "force") {
        char block;
        cout << "Please enter a block to force." << endl;
        
        while (true) {
         cin >> block;
         if (cin.fail()) {
          cin.clear();
          cin.ignore();
          continue;
          }
         
         if (validBlock(block)) break;
         cout << "Invalid block. Please enter I, J, L, T, S, Z, or O." << endl;
         }        
         game->force(block); 
        } else if ((action == "heavy")||(action == "blind")) {
         game->enableSpecialAction(action);
        }
        game->print();
       }

       // error checking
        
        cin >> command;
        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            continue;
        }

        int step = getNumPrefix(command);
        string translatedCmd = cmdMappings[command]; // look up command in dictionary
        if (translatedCmd == " ") continue;

        if (translatedCmd == "left") {
            game->move("left", step);
        } else if (translatedCmd == "right") {
            game->move("right", step);
        } else if (translatedCmd == "down") {
            game->move("down", step);
        } else if (translatedCmd == "drop") {
            game->drop();
        } else if (translatedCmd == "level") { // needs to be looked into
            int level;
            cin >> level;
            game->setLevel(level);
        } else if (translatedCmd == "clockwise") {
            game->rotate(false, step);
        } else if (translatedCmd == "counterclockwise") {
            game->rotate(true, step);
        } else if (translatedCmd == "levelup") {
            // get level and increment, use step
        } else if (translatedCmd == "leveldown") {
            // get level and decrement, use step
        } else if (translatedCmd == "norandom") {
            string randomFile;
            cin >> randomFile;
            // use this file
        } else if (translatedCmd == "random") {
        } else if (translatedCmd == "sequence") {
            string sequenceFile;
            cin >> sequenceFile;
        } else if ((translatedCmd == "I")||(translatedCmd == "J")||(translatedCmd == "L")||
        (translatedCmd == "O")||(translatedCmd == "T")||(translatedCmd == "S")||(translatedCmd == "Z")) {
        } else if (translatedCmd == "restart") {
            game->restart();
        }
    } 

}
