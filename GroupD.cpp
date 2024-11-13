#include <iostream>      // Provides input and output stream objects such as cin, cout
#include <vector>        // Provides the vector container, a dynamic array
#include <string>        // Provides the string class for handling text
#include <fstream>       // Provides file stream objects for file input and output
#include <thread>        // Provides threading support, used for sleep functionality
#include <chrono>        // Provides time-related functions, used for defining sleep durations
#include <cstdlib>       // Provides general purpose functions, including random number generation
#include <algorithm>     // Provides algorithms like transform for manipulating ranges of elements


using namespace std;

// Base class for the game containing common attributes and methods
class BaseGame {
public:
    vector<string> inventory;  // Player's inventory to store items
    string username;           // Player's username
    string gender;             // Player's gender
    int age;                   // Player's age
    string difficulty;         // Game difficulty based on age

    // Function to pause execution for a specified number of milliseconds
    void sleep(int milliseconds) {
        this_thread::sleep_for(chrono::milliseconds(milliseconds));
    }
};

// Derived class handling conditions like game failure and ending
class Conditions : public BaseGame {
public:
    // Function to handle failure scenarios
    void failed_game(const string &username) {
        cout << "\nYou failed to find the treasure, " << username << "!" << endl;
        sleep(3000);
        exit(0);  // Terminate the program
    }

    // Function to handle successful game ending
    void end_game(const string &username, const vector<string> &inventory) {
        cout << "\nCongratulations! You found the treasure, " << username << "!" << endl;
        sleep(3000);
        saveGame(username, inventory);  // Save the game state before ending
        exit(0);  // Terminate the program
    }

private:
    // Private function to save the game state to a file
    void saveGame(const string &username, const vector<string> &inventory) {
        ofstream saveFile(username + "_save.txt");
        for (const string &item : inventory) {
            saveFile << item << endl;  // Save each inventory item to the file
        }
        saveFile.close();
        cout << "Game saved successfully." << endl;
    }
};

// Forward declarations of scene classes
class Game_Scene1;
class Game_Scene2;

// Class for the game's introductory scene
class Intro : public BaseGame {
public:
    // Function to start the game by asking if the player wants to start a new game or load an existing game
    void start() {
        string choice;
        cout << "Do you want to start a new game or load an existing game? (New/Load): ";
        cin >> choice;
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower); // Convert input to lowercase

        if (choice == "new") {
            newGame();
        } else if (choice == "load") {
            cout << "Enter your username: ";
            cin >> username;
            loadGame();
        } else {
            cout << "Invalid choice. Please try again." << endl;
            start();
        }
    }

    // Function to start a new game by taking username input and showing the introduction
    void newGame() {
        cout << "Enter your username: ";
        cin >> username;
        username[0] = toupper(username[0]); // Capitalize the first letter of the username
        createProfile();  // Create the user profile
    }

    // Function to create a new user profile
    void createProfile() {
        cout << "Enter your gender (M/F): ";
        cin >> gender;
        cout << "Enter your age: ";
        cin >> age;
        if (age < 12) {
            difficulty = "Easy";
            cout<<"Easy mode"<<endl;
            sleep(2000);
        } else if (age < 18) {
            difficulty = "Medium";
            cout<<"Medium mode"<<endl;
            sleep(2000);
        } else {
            difficulty = "Hard";
            cout<<"Hard mode"<<endl;
            sleep(2000);
        }
        saveProfile();  // Save the profile information
        showIntro();    // Display the introductory narrative
    }

    // Function to load an existing game from a save file
    void loadGame();

    // Function to save the current game state to a file
    void saveGame() {
        ofstream saveFile(username + "_save.txt");
        for (const string &item : inventory) {
            saveFile << item << endl;  // Save each inventory item to the file
        }
        saveFile.close();
        cout << "Game saved successfully." << endl;
    }

private:
    // Private function to save the user profile to a file
    void saveProfile() {
        ofstream profileFile(username + "_profile.txt");
        profileFile << username << endl;
        profileFile << gender << endl;
        profileFile << age << endl;
        profileFile << difficulty << endl;
        profileFile.close();
    }

    // Private function to show the game introduction
    void showIntro() {
        cout << "Welcome to 'The Lost Kingdom of Eldarath'!" << endl;
        sleep(3500);
        cout << "You are a brave adventurer seeking to uncover the secrets of the island of Eldarath." << endl;
        sleep(4500);
        cout << "Your journey begins in the small village of Willowdale, where rumors of a legendary treasure have drawn you." << endl;
        sleep(3500);
        cout << "You enter a tavern where you encounter a Village Elder." << endl;
        sleep(3000);

        string action;
        while (true) {
            cout << "Engage in a conversation with the Elder (type:Talk): ";
            cin >> action;
            if (action == "Talk" || action == "talk") {
                the_village_elder();  // If player chooses to talk, proceed with elder interaction
                break;
            } else {
                cout << "Invalid input. Try again." << endl;
                showIntro() ;
            }
        }
        
    }

    // Private function to handle interaction with the village elder
    void the_village_elder();
};

// Class for handling the first game scene involving the map and riddle
class Game_Scene1 : public Conditions {
public:
    // Function to handle interactions involving the map
    void the_map(vector<string> &inv, string &user) {
        inventory = inv;  // Copy the inventory and username
        username = user;

        cout << "As you leave the tavern, you enter a local market where you approach a mysterious merchant." << endl;
        sleep(4500);
        cout << "You spot an old map among  his goods and purchase it." << endl;
        sleep(4000);
        cout << "Inscribed on the map are locations on the island and a riddle." << endl;
        sleep(3500);
        the_riddle();  // Proceed to solve the riddle
    }

    // Private function to present and handle the riddle
    void the_riddle();
};

// Class for handling the second game scene involving the castle journey and gate question
class Game_Scene2 : public Conditions {
public:
    // Function to handle the journey to the castle and encounter with a monster
    void castle_journey(vector<string> &inv, string &user) {
        inventory = inv;  // Copy the inventory and username
        username = user;

        cout << "You journey out of the village towards the castle."<<endl;
        sleep(4500);
        cout << "On your way, you encounter a fierce monster blocking your path!" << endl;
        sleep(4500);
        cout << "Prepare for battle!" << endl;
        int monster_health = (difficulty == "Easy") ? 30 : (difficulty == "Medium") ? 50 : 70;
        int player_health = 100;

        while (monster_health > 0 && player_health > 0) {
            cout << "\nYour health: " << player_health << endl;
            cout << "Monster's health: " << monster_health << endl;

            string action;
            cout << "Do you want to 'attack' or 'defend'? ";
            cin >> action;
            transform(action.begin(), action.end(), action.begin(), ::tolower); // Convert input to lowercase

            if (action == "attack") {
                int damage = rand() % 11 + 10;  // Random damage between 10 and 20
                monster_health -= damage;
                cout << "You attack the monster, dealing " << damage << " damage." << endl;
            } else if (action == "defend") {
                int heal = rand() % 11 + 5;  // Random heal between 5 and 15
                player_health += heal;
                cout << "You defend and heal yourself for " << heal << " health." << endl;
            } else {
                cout << "Invalid action. Try again." << endl;
                continue;
            }

            if (monster_health > 0) {
                int monster_damage = rand() % 11 + 5;  // Random monster damage between 5 and 15
                player_health -= monster_damage;
                cout << "The monster attacks you, dealing " << monster_damage << " damage." << endl;
            }
        }

        if (player_health > 0) {
            cout << "You have defeated the monster and continue your journey to the castle." << endl;
            castle_gate();  // Proceed to the castle gate
        } else {
            failed_game(username);  // Handle game failure if player health drops to zero
        }
    }

private:
    // Private function to handle the interaction at the castle gate
    void castle_gate() {
        cout << "You arrive at the castle gate. An ancient stone golem guards the entrance and speaks: " << endl;
        sleep(4000);
        cout << "'Answer this question to pass: What belongs to you, but others use it more than you do?'" << endl;
        sleep(3500);
        string answer;
        cout << "What is your answer? ";
        cin >> answer;
        transform(answer.begin(), answer.end(), answer.begin(), ::tolower); // Convert input to lowercase

        if (answer == "name") {
            cout << "The stone golem steps aside, allowing you to enter the castle. You have found the treasure!" << endl;
            end_game(username, inventory);  // Handle game ending if answer is correct
        } else {
            cout << "Incorrect answer. You are not allowed to pass." << endl;
            failed_game(username);  // Handle game failure if answer is incorrect
        }
    }
};

// Implementations of member functions that require complete types
void Intro::loadGame() {
    ifstream saveFile(username + "_save.txt");
    if (saveFile) {
        cout << "Loading saved game for " << username << "..." << endl;
        string item;
        // Load inventory items from the save file
        while (getline(saveFile, item)) {
            inventory.push_back(item);
        }
        saveFile.close();
        cout << "Game loaded successfully." << endl;
        Game_Scene1 scene1;
        scene1.the_map(inventory, username);  // Continue the game from where it was saved
    } else {
        cout << "No saved game found for " << username << ". Starting a new game." << endl;
        newGame();  // Start a new game if no save file is found
    }
}

void Intro::the_village_elder() {
    cout << "Village Elder: 'Ah, a new adventurer! I see you seek the legendary treasure of Eldarath.'" << endl;
    sleep(3000);
    cout << "Village Elder: 'You will need a map, it will guide you to the castle where the treasure is hidden."<<endl;
    sleep(3500);
    cout<<"But beware, the path is fraught with danger.'" << endl;
    sleep(2000);
    inventory.push_back("Map");
    Game_Scene1 scene1;
    scene1.the_map(inventory, username);  // Continue the game to the map scene
}

// Function to handle the riddle scene in the map
void Game_Scene1::the_riddle() {
    cout << "The riddle on the map reads:"<<endl;
    sleep(2000);
    cout<<"'I have a needle, but I do not sew. I point the way as I turn to and fro. What am I?'" << endl;
    sleep(2500);
    string answer;
    cout << "What is your answer? ";
    cin >> answer;
    transform(answer.begin(), answer.end(), answer.begin(), ::tolower); // Convert input to lowercase

    if (answer == "compass") {
        cout << "Correct! The map reveals a hidden path leading to the castle." << endl;
        Game_Scene2 scene2;
        scene2.castle_journey(inventory, username);  // Proceed to the castle journey scene
    } else {
        cout << "Incorrect answer. The map's secrets remain hidden." << endl;
        failed_game(username);  // Handle game failure if answer is incorrect
    }
}

// Main function to start the game
int main() {
    Intro gameIntro;
    gameIntro.start();
    return 0;
}
/*
Group D:
1.Armah Rhoda-1801822
2.Matthew Senahey - 1807522
3.Anaba Naama Selma-1801322
4.Azorliade Elinam Erika-1802622
5.Jane-Belle Fafa Agbeti-1800222
6.Hasford Robert Henry - 1804822
7.Agyei David Beckham - 1800422
8.Addae Yvette - 1799622
9.Nana Kwadwo Oduro Aboakye-Atta - 1799522

*/