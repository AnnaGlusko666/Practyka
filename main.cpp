#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;

char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
char current_marker;
int current_player;
int move_count = 0;
int player1_wins = 0;
int player2_wins = 0;
std::ofstream logFile;

void log(const std::string& message) {
    std::time_t now = std::time(nullptr);
    char* dt = std::ctime(&now);
    dt[std::strlen(dt) - 1] = '\0';
    logFile << "\"" << dt << "\",\"" << message << "\"\n";
}

void drawBoard() {
    std::cout << " " << std::setw(1) << board[0][0] << " | " << std::setw(1) << board[0][1] << " | " << std::setw(1) << board[0][2] << "\n";
    std::cout << " " << std::setw(2) << "---" << "|" << std::setw(1) << "---" << "|" << std::setw(1) << "---" << "\n";
    std::cout << " " << std::setw(2) << board[1][0] << " | " << std::setw(1) << board[1][1] << " | " << std::setw(1) << board[1][2] << "\n";
    std::cout << " " << std::setw(2) << "---" << "|" << std::setw(1) << "---" << "|" << std::setw(1) << "---" << "\n";
    std::cout << " " << std::setw(2) << board[2][0] << " | " << std::setw(1) << board[2][1] << " | " << std::setw(1) << board[2][2] << "\n";
}

bool placeMarker(int slot) {
    int row = (slot - 1) / 3;
    int col = (slot - 1) % 3;

    if (board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = current_marker;
        return true;
    } else {
        return false;
    }
}

int winner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return current_player;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return current_player;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return current_player;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return current_player;
    }
    return 0;
}

void swapPlayerAndMarker() {
    if (current_marker == 'X') {
        current_marker = 'O';
    } else {
        current_marker = 'X';
    }
    if (current_player == 1) {
        current_player = 2;
    } else {
        current_player = 1;
    }
}

void saveGame() {
    json game_state;
    game_state["board"] = { { board[0][0], board[0][1], board[0][2] },
                            { board[1][0], board[1][1], board[1][2] },
                            { board[2][0], board[2][1], board[2][2] } };
    game_state["current_marker"] = current_marker;
    game_state["current_player"] = current_player;
    game_state["move_count"] = move_count;
    game_state["player1_wins"] = player1_wins;
    game_state["player2_wins"] = player2_wins;

    std::ofstream outFile("game_state.json");
    outFile << game_state.dump(4);
    outFile.close();

    std::cout << "Game saved!\n";
    log("Game saved.");
}

void loadGame() {
    std::ifstream inFile("game_state.json");

    if (inFile) {
        json game_state;
        inFile >> game_state;
        inFile.close();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = game_state["board"][i][j].get<char>();
            }
        }
        current_marker = game_state["current_marker"].get<char>();
        current_player = game_state["current_player"];
        move_count = game_state["move_count"];
        player1_wins = game_state["player1_wins"];
        player2_wins = game_state["player2_wins"];

        std::cout << "Game loaded!\n";
        log("Game loaded.");
    } else {
        std::cout << "Error loading game!\n";
        log("Error loading game.");
    }
}

void resetBoard() {
    char initial_board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
    std::memcpy(board, initial_board, 9 * sizeof(char));
    move_count = 0;
}

void game() {
    logFile.open("game_log.csv", std::ios::app);
    log("Game started.");
    std::cout << "Do you want to load a saved game? (y/n): ";
    char loadChoice;
    std::cin >> loadChoice;
    if (loadChoice == 'y') {
        loadGame();
        int result = winner();
        if (result != 0 || move_count >= 9) {
            std::cout << "Loaded game is already completed. Start a new game.\n";
            resetBoard();
        }
    } else {
        std::cout << "Player 1, choose your marker (X or O): ";
        char marker_p1;
        std::cin >> marker_p1;

        while (marker_p1 != 'X' && marker_p1 != 'O') {
            std::cout << "Invalid marker! Please choose X or O: ";
            std::cin >> marker_p1;
        }

        current_player = 1;
        current_marker = marker_p1;
        log("Player 1 chose marker " + std::string(1, marker_p1));
    }

    bool play_again = true;
    while (play_again) {
        if (loadChoice != 'y' || move_count == 0) {
            resetBoard();
        }
        drawBoard();

        int player_won = 0;

        for (int i = move_count; i < 9; i++) {
            std::cout << "Player " << current_player << ", choose a slot for your marker (or press 0 to save the game): ";
            int slot;
            std::cin >> slot;

            if (slot == 0) {
                saveGame();
                return; // End the game after saving
            }

            if (slot < 1 || slot > 9) {
                std::cout << "Invalid slot! Try again.\n";
                log("Player " + std::to_string(current_player) + " chose an invalid slot " + std::to_string(slot));
                i--;
                continue;
            }
            if (!placeMarker(slot)) {
                std::cout << "This slot is occupied! Try again.\n";
                log("Player " + std::to_string(current_player) + " tried an occupied slot " + std::to_string(slot));
                i--;
                continue;
            }

            move_count++;
            log("Player " + std::to_string(current_player) + " placed a marker in slot " + std::to_string(slot));
            drawBoard();
            player_won = winner();

            if (player_won == 1) {
                player1_wins++;
                std::cout << "Player 1 won in " << move_count << " moves!\n";
                log("Player 1 won!");
                break;
            }
            if (player_won == 2) {
                player2_wins++;
                std::cout << "Player 2 won in " << move_count << " moves!\n";
                log("Player 2 won!");
                break;
            }

            swapPlayerAndMarker();
        }

        if (player_won == 0 && move_count >= 9) {
            std::cout << "Draw!\n";
            log("Game ended in a draw.");
        }

        std::cout << "Player 1 wins: " << player1_wins << "\n";
        std::cout << "Player 2 wins: " << player2_wins << "\n";

        std::cout << "Do you want to play again? (y/n): ";
        char playAgainChoice;
        std::cin >> playAgainChoice;
        if (playAgainChoice != 'y') {
            play_again = false;
        } else {
            resetBoard();
            move_count = 0;
        }
    }

    logFile.close();
}

int main() {
    game();
    return 0;
}