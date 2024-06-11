#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>

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
    logFile << "[" << dt << "] " << message << "\n";
}

void drawBoard() {
    std::cout << " " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << "\n";
    std::cout << "---|---|---\n";
    std::cout << " " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << "\n";
    std::cout << "---|---|---\n";
    std::cout << " " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << "\n";
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
    std::ofstream boardFile("board.txt");
    std::ofstream playerFile("player.txt");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            boardFile << board[i][j];
        }
    }
    boardFile.close();

    playerFile << current_player << ' ' << current_marker << ' ' << move_count;
    playerFile.close();

    std::cout << "Гра збережена!\n";
    log("Гра збережена.");
}

void loadGame() {
    std::ifstream boardFile("board.txt");
    std::ifstream playerFile("player.txt");

    if (boardFile && playerFile) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                boardFile >> board[i][j];
            }
        }
        boardFile.close();

        playerFile >> current_player >> current_marker >> move_count;
        playerFile.close();

        std::cout << "Гра завантажена!\n";
        log("Гра завантажена.");
    } else {
        std::cout << "Помилка завантаження гри!\n";
        log("Помилка завантаження гри.");
    }
}

void resetBoard() {
    char initial_board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
    std::memcpy(board, initial_board, 9 * sizeof(char));
    move_count = 0;
}

void game() {
    logFile.open("game_log.txt", std::ios::app);
    log("Гра почалася.");
    std::cout << "Чи хочете ви завантажити збережену гру? (y/n): ";
    char loadChoice;
    std::cin >> loadChoice;

    if (loadChoice == 'y') {
        loadGame();
        int result = winner();
        if (result != 0 || move_count >= 9) {
            std::cout << "Завантажена гра вже завершена. Розпочніть нову гру.\n";
            resetBoard();
        }
    } else {
        std::cout << "Гравець 1, оберіть ваш маркер (X або O): ";
        char marker_p1;
        std::cin >> marker_p1;

        while (marker_p1 != 'X' && marker_p1 != 'O') {
            std::cout << "Невірний маркер! Будь ласка, оберіть X або O: ";
            std::cin >> marker_p1;
        }

        current_player = 1;
        current_marker = marker_p1;
        log("Гравець 1 обрав маркер " + std::string(1, marker_p1));
    }

    bool play_again = true;
    while (play_again) {
        if (loadChoice != 'y' || move_count == 0) {
            resetBoard();
        }
        drawBoard();

        int player_won = 0;

        for (int i = move_count; i < 9; i++) {
            std::cout << "Гравець " << current_player << ", оберіть слот для вашого маркера (або натисніть 0 для збереження гри): ";
            int slot;
            std::cin >> slot;

            if (slot == 0) {
                saveGame();
                return; // Завершение игры после сохранения
            }

            if (slot < 1 || slot > 9) {
                std::cout << "Невірний слот! Спробуйте ще раз.\n";
                log("Гравець " + std::to_string(current_player) + " обрав невірний слот " + std::to_string(slot));
                i--;
                continue;
            }
            if (!placeMarker(slot)) {
                std::cout << "Цей слот занят! Спробуйте ще раз.\n";
                log("Гравець " + std::to_string(current_player) + " спробував зайнятий слот " + std::to_string(slot));
                i--;
                continue;
            }

            move_count++;
            log("Гравець " + std::to_string(current_player) + " поставив маркер в слот " + std::to_string(slot));
            drawBoard();
            player_won = winner();

            if (player_won == 1) {
                player1_wins++;
                std::cout << "Гравець 1 переміг за " << move_count << " ходів!\n";
                log("Гравець 1 переміг!");
                break;
            }
            if (player_won == 2) {
                player2_wins++;
                std::cout << "Гравець 2 переміг за " << move_count << " ходів!\n";
                log("Гравець 2 переміг!");
                break;
            }

            swapPlayerAndMarker();
        }

        if (player_won == 0 && move_count >= 9) {
            std::cout << "Нічия!\n";
            log("Гра закінчилася нічиєю.");
        }

        std::cout << "Гравець 1 перемог: " << player1_wins << "\n";
        std::cout << "Гравець 2 перемог: " << player2_wins << "\n";

        std::cout << "Чи хочете ви зіграти ще раз? (y/n): ";
        char playAgainChoice;
        std::cin >> playAgainChoice;
        if (playAgainChoice != 'y') {
            play_again = false;
        } else {
            resetBoard();
            move_count = 0; // Reset move_count when starting a new game
        }
    }

    logFile.close();
}

int main() {
    game();
    return 0;
}
