# Гра "Хрестики-Нулики" з функціями збереження та завантаження

Ця програма реалізує гру "Хрестики-Нулики" з можливістю збереження та завантаження гри, веденням журналу подій та обрахунком перемог для кожного гравця.

## Опис файлів

### main.cpp
Основний файл з кодом гри.

#### Основні компоненти коду

- **Змінні:**
  - `char board[3][3]`: Матриця 3x3 для зберігання стану ігрового поля.
  - `char current_marker`: Маркер поточного гравця ('X' або 'O').
  - `int current_player`: Поточний гравець (1 або 2).
  - `int move_count`: Кількість зроблених ходів.
  - `int player1_wins`, `int player2_wins`: Лічильники перемог для кожного гравця.
  - `std::ofstream logFile`: Файл для журналу подій.

- **Функції:**
  - `void log(const std::string& message);`: Записує повідомлення до файлу журналу з позначкою часу.
  - `void drawBoard();`: Відображає поточний стан ігрового поля.
  - `bool placeMarker(int slot);`: Розміщує маркер на ігровому полі, якщо вибраний слот не зайнятий.
  - `int winner();`: Перевіряє, чи є переможець. Повертає номер поточного гравця, якщо він виграв, або 0, якщо немає переможця.
  - `void swapPlayerAndMarker();`: Змінює поточного гравця та його маркер.
  - `void saveGame();`: Зберігає поточний стан гри до файлів `board.txt` та `player.txt`.
  - `void loadGame();`: Завантажує стан гри з файлів `board.txt` та `player.txt`.
  - `void resetBoard();`: Скидає ігрове поле до початкового стану.
  - `void game();`: Основний цикл гри. Містить логіку для гри, збереження та завантаження гри.

- **Основна функція:**
  ```cpp
  int main() {
      game();
      return 0;
  }
  ```
  Запускає гру.

## Взаємодія з користувачем

- Користувач обирає маркер для гравця 1 ('X' або 'O').
- Кожен гравець по черзі вибирає слот для свого маркера.
- Користувач може зберегти гру в будь-який момент, натиснувши 0.
- Гра автоматично перевіряє наявність переможця або нічиєї.
- Після закінчення гри користувач може вибрати, чи хоче він грати знову.

## Ведення журналу

Програма веде журнал подій у файлі `game_log.txt`, записуючи всі важливі дії гравців та результати гри.

## Збереження та завантаження

Програма може зберігати та завантажувати стан гри з файлів `board.txt` та `player.txt`, що дозволяє продовжити гру пізніше.

## Приклад використання

1. Запустіть програму.
2. Виберіть маркер для гравця 1.
3. Грайте в хрестики-нулики, вибираючи слоти для маркерів.
4. Збережіть гру, натиснувши 0.
5. Завантажте збережену гру, вибравши 'y' при запиті на завантаження гри.

Цей код забезпечує просту та функціональну гру "Хрестики-Нулики" з можливістю збереження та відновлення гри, що дозволяє гравцям продовжити гру з того місця, де вони зупинилися.
## Приклади роботи гри
![image](https://github.com/AnnaGlusko666/Practyka/assets/172313784/c1ecf2e3-3ff2-455e-aa4f-c54e2c33963c)
![image](https://github.com/AnnaGlusko666/Practyka/assets/172313784/26162826-b3e6-4c47-b2d0-7048545dcc07)

