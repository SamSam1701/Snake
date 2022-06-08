#include "snakeGame.h"

POINT operator + (POINT const& a, POINT const& b) {
    return { a.x + b.x,a.y + b.y };
}

bool operator == (POINT const& a, POINT const& b) {
    return (a.x == b.x) && (a.y == b.y);
}

void drawRock() {
    for (int i = 0; i < min(level, 5) * 3; i++) {
        GotoXY(rock[i].x, rock[i].y); printf(ROCK);
    }
}

bool checkRock(int x, int y) {

    for (int i = 0; i < SIZE_SNAKE; i++)
        if (x <= snake[i].x && snake[i].x <= x + strlen(ROCK) - 1 && snake[i].y == y)
            return 0;

    if (x <= food[FOOD_INDEX].x && food[FOOD_INDEX].x <= x + strlen(ROCK) - 1 && food[FOOD_INDEX].y == y)
        return 0;

    return 1;
}

void createRock() {
    int x, y;

    for (int i = 0; i < min(level, 5) * 3; i++) {
        do {
            x = rand() % (WIDTH_CONSOLE - strlen(ROCK)) + 1;
            y = rand() % (HEIGH_CONSOLE - strlen(ROCK)) + 1;
        } while (checkRock(x, y) == false);

        rock[i] = { x,y };

        GotoXY(x, y); printf(ROCK);
    }
}


void drawScore() {

    ifstream inp("ScoreBoard.txt");
    vector<pair<int, string>> s;
    string name;
    int sc;
   
    for (int i = 0; i < 21; i++)
    {
        GotoXY(WIDTH_CONSOLE + 3, i);
        cout << char(221);
        GotoXY(WIDTH_CONSOLE + 46, i);
        cout << char(221);
       
    }
    GotoXY(WIDTH_CONSOLE + 3, 0);
    for (int i = 0; i < 44; i++)
    {
       
        cout << char(220);
      
    }
    GotoXY(WIDTH_CONSOLE + 3, 20);
    for (int i = 0; i < 44; i++)
    {

        cout << char(220);

    }


    GotoXY(WIDTH_CONSOLE + 22, 2); cout << "Top 5";

    if (!inp) return;

    while (inp >> sc >> name)
        s.push_back({ sc,name });

    inp.close();

    sort(s.rbegin(), s.rend());
     
    for (int i = 0; i < s.size() && i < 5; ++i) {
        GotoXY(WIDTH_CONSOLE + 21, i + 4);
        cout  << s[i].second << "\t" << s[i].first;// << s.size();
    }

}

void drawSnake() {
    for (int i = 0; i < SIZE_SNAKE; i++) {
        GotoXY(snake[i].x, snake[i].y);
        cout << MSSV[i % strlen(MSSV)];
    }
}

void drawFood() {
    char s = MSSV[(SIZE_SNAKE) % strlen(MSSV)];

    GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);

    cout << s;
}

void drawGate() {
    char s = 254;
    GotoXY(gate[0].x, gate[0].y);
    cout << s;
    for (int i = 1; i < 5; i++) {
        GotoXY(gate[i].x, gate[i].y);
        cout << "X";
    }
}

void clearGate() {
    for (int i = 0; i < 5; i++) {
        GotoXY(gate[i].x, gate[i].y);
        cout << " ";
    }
}

void effectDead() {

    drawSnake();

    for (int i = 0; i < 10; ++i) {
        for (int i = 0; i < SIZE_SNAKE; ++i) {
            GotoXY(snake[i].x, snake[i].y);
            cout << "-"; Sleep(5);
        }
        for (int i = 0; i < SIZE_SNAKE; ++i) {
            GotoXY(snake[i].x, snake[i].y);
            cout << "\\"; Sleep(5);
        }
        for (int i = 0; i < SIZE_SNAKE; ++i) {
            GotoXY(snake[i].x, snake[i].y);
            cout << "|"; Sleep(5);
        }
        for (int i = 0; i < SIZE_SNAKE; ++i) {
            GotoXY(snake[i].x, snake[i].y);
            cout << "/"; Sleep(5);
        }

    }

    drawSnake();
}

void ShowCur(bool CursorVisibility) // Ham xoa con tro chuot
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
    SetConsoleCursorInfo(handle, &cursor);
}

void SetColor(WORD color) //Ham tao mau
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;

    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

int Move(POINT step) {
    step = step + snake[SIZE_SNAKE - 1];

    if (step.x * step.y == 0 || step.x == WIDTH_CONSOLE || step.y == HEIGH_CONSOLE)
        return ProcessDead(), 0;

    for (int i = 0; i < SIZE_SNAKE; i++)
        if (step == snake[i])
            return ProcessDead(), 0;

    for (int i = 0; i < min(level, 5) * 3; i++)
        if (rock[i].x <= step.x && step.x <= rock[i].x + strlen(ROCK) - 1 && rock[i].y == step.y)
            return ProcessDead(), 0;

    if (food_ate == MAX_SPEED * MAX_SIZE_FOOD)
    {
        for (int i = 1; i < 5; i++)
            if (gate[i] == step)
                return ProcessDead(), 0;
    };

    if (step == food[FOOD_INDEX])
    {
        food_ate++;
        score++;
        snake[SIZE_SNAKE] = food[FOOD_INDEX];
        SIZE_SNAKE++;
        Eat1();
        if (FOOD_INDEX < MAX_SIZE_FOOD - 1)FOOD_INDEX++;
    }
    else
    {
        for (int i = 1; i < SIZE_SNAKE; i++)
            snake[i - 1] = snake[i];

        snake[SIZE_SNAKE - 1] = step;
        GotoXY(0, HEIGH_CONSOLE + 1);
        cout << "Score: " << score << setw(10) << "level: " << level << setw(10) << "Speed: " << SPEED;
        Eat2();
    }
}

void saveGame() {
    string s;

    GotoXY(0, HEIGH_CONSOLE + 2);

    cout << "===SAVE GAME" << endl;
    cout << "File name: "; cin >> s;

    ofstream out(s + ".GAME");

    out << CHAR_LOCK << " " << MOVING << " " << SPEED << " " << FOOD_INDEX << " " << WIDTH_CONSOLE << " " << HEIGH_CONSOLE << " " << SIZE_SNAKE << " ";

    for (int i = 0; i < SIZE_SNAKE; ++i)
        out << snake[i].x << " " << snake[i].y << " ";

    out << FOOD_INDEX << " ";

    for (int i = 0; i < 4; ++i) out << food[i].x << " " << food[i].y << " ";

    out << STATE << " " << score << " " << level << " " << food_ate << " ";

    for (int i = 0; i < min(level, 5) * 3; ++i)
        out << rock[i].x << " " << rock[i].y << " ";

    out.close();
}

void GuideToPlay()
{
    system("cls");
    cout << "\t\t\t\t\tHOW TO PLAY!!\n\n\n";
    cout << "\t\tThe player controls the snake to move away from the boundary lines. \n";
    cout << "\t\tIf it collides with the boundary line or obstacles, the game will be over.\n";
    cout << "\t\tUse the A keys to go left, S to go down, W to go upand D to go right.\n";
    cout << "\t\tAfter 4 times of eating food, the game will be more difficult";
    cout << "\n\nPRESS 1 TO PLAY NEW GAME ";
    cout << "\n\nPRESS 2 TO OPEN SAVED GAME ";
    cout << "\n\nYour Choice: ";

}

int check() {

    string s;
    cout << "===LOAD GAME===" << endl;
    cout << "File name: "; cin >> s;

    ifstream inp(s + ".GAME");

    if (!inp) {

        cout << "Tro choi khong co, chung ta bat dau tu dau!\n\n";
        system("pause");
        NewGame();
        return 0;
    }

    inp >> CHAR_LOCK >> MOVING >> SPEED >> FOOD_INDEX >> WIDTH_CONSOLE >> HEIGH_CONSOLE >> SIZE_SNAKE;

    for (int i = 0; i < SIZE_SNAKE; ++i)
        inp >> snake[i].x >> snake[i].y;

    inp >> FOOD_INDEX;

    for (int i = 0; i < 4; ++i) inp >> food[i].x >> food[i].y;

    inp >> STATE >> score >> level >> food_ate;

    for (int i = 0; i < min(level, 5) * 3; ++i)
        inp >> rock[i].x >> rock[i].y;

    inp.close();

    system("cls");

    DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE);
    drawRock();
}

void startGame() { // Bắt đầu chơi
    system("cls");

    int k;
    cout << "\n\n\n";
    cout << "\t\t\t   _____             _                                     " << endl;
    cout << "\t\t\t  / ____|           | |                                    " << endl;
    cout << "\t\t\t | (___  _ __   __ _| | _____    __ _  __ _ _ __ ___   ___ " << endl;
    cout << "\t\t\t  \\___ \\| '_ \\ / _` | |/ / _ \\  / _` |/ _` | '_ ` _ \\ / _ \\" << endl;
    cout << "\t\t\t  ____) | | | | (_| |   |  __/ | (_| | (_| | | | | | |  __/" << endl;
    cout << "\t\t\t |_____/|_| |_|\\__,_|_|\\_\\___|  \\__, |\\__,_|_| |_| |_|\\___|" << endl;
    cout << "\t\t\t                                 __/ |                     " << endl;
    cout << "\t\t\t                                |___/                      " << endl;

    cout << "\n\n\n\n\n";
    cout << "\t\t\t\t=====MENU=====\t\t\t====NHOM 19====\n";
    cout << "\t\t\t\t1. NEW GAME\t\t\tTran Quoc Huy \n";
    cout << "\t\t\t\t2. OPEN SAVE GAME\t\tHo Minh Quan\n";
    cout << "\t\t\t\t3. HOW TO PLAY\t\t\tLe Duc Thien\n";
    cout << "\t\t\t\t\t\t\t\tNguyen Phuoc Sam\n";
    cout << "\t\t\t\t\t\t\t\tPham Thanh Thien\n";

    cout << "\n\nEnter your selection: ";
    while (cin >> k)
    {
        if (k == 1)
        {
            NewGame();
            break;
            GotoXY(0, HEIGH_CONSOLE + 1);
            cout << "Score: " << score << setw(10) << "level: " << level << setw(10) << "Speed: " << SPEED;
        }
        else if (k == 2)
        {
            check();
            break;
            GotoXY(0, HEIGH_CONSOLE + 1);
            cout << "Score: " << score << setw(10) << "level: " << level << setw(10) << "Speed: " << SPEED;
        }
        else if (k == 3)
        {
            GuideToPlay();
        }
        else
        {
            cout << "The selection not true!" << endl;
            cout << "Enter your selection: " << endl;
        }
    }

}

void FixConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);

}

void GotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool IsValid(int x, int y) {

    for (int i = 0; i < SIZE_SNAKE; i++)
        if (snake[i].x == x && snake[i].y == y)
            return false;

    for (int i = 0; i < min(level, 5) * 3; i++)
        if (rock[i].x <= x && x <= rock[i].x + strlen(ROCK) - 1 && rock[i].y == y)
            return 0;

    return true;
}

bool IsValid2(POINT gate[]) {

    for (int i = 0; i < SIZE_SNAKE; i++)
        for (int j = 0; j < 5; j++)
            if (snake[i].x == gate[j].x && snake[i].y == gate[j].y)
                return false;

    for (int i = 0; i < min(level, 5) * 3; i++)
        for (int j = 0; j < 5; j++)
            if (rock[i].x <= gate[j].x && gate[j].x <= rock[i].x + strlen(ROCK) - 1 && rock[i].y == gate[j].y)
                return 0;

    return true;
}

void GenerateFood() {
    int x, y;

    for (int i = 0; i < MAX_SIZE_FOOD; i++) {
        do {
            x = rand() % (WIDTH_CONSOLE - 1) + 1;
            y = rand() % (HEIGH_CONSOLE - 1) + 1;
        } while (IsValid(x, y) == false);

        food[i] = { x,y };
    }
}

void GenerateGate() {
    int x, y;
    do {
        x = rand() % (WIDTH_CONSOLE - 5) + 3;
        y = rand() % (HEIGH_CONSOLE - 5) + 3;
        gate[0] = { x,y };
        gate[1] = { x - 2,y };
        gate[2] = { x + 2,y };
        gate[3] = { x,y - 1 };

    } while (IsValid2(gate));
}

void ResetData() {
    //Khởi tạo các giá trị toàn cục
    CHAR_LOCK = 'A', MOVING = 'D', SPEED = 1; FOOD_INDEX = 0, WIDTH_CONSOLE = 70,
        HEIGH_CONSOLE = 20, SIZE_SNAKE = 6;

    score = 0; level = 0; food_ate = 0;

    //Khởi tạo giá trị mặc định cho snake
    snake[0] = { 10, 5 }; snake[1] = { 11, 5 };
    snake[2] = { 12, 5 }; snake[3] = { 13, 5 };
    snake[4] = { 14, 5 }; snake[5] = { 15, 5 };
    GenerateFood();//Tạo mảng thức ăn food
}

void NewGame() {
    system("cls");
    ResetData(); // Khởi tạo dữ liệu gốc
    DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE); // Vẽ màn hình game
    STATE = 1;//Bắt đầu cho Thread chạy
    createRock();
    GenerateGate();
}

void DrawBoard(int x, int y, int width, int height) {
    int curPosX = 0; int curPosY = 0;
    GotoXY(x, y); cout << char(220);
    for (int i = 1; i < width; i++)cout << char(220);
    cout << char(221);
    GotoXY(x, height + y); cout << char(221);
    for (int i = 1; i < width; i++)cout << char(220);
    cout << char(221);
    for (int i = y + 1; i < height + y; i++) {
        GotoXY(x, i); cout << char(221);
        GotoXY(x + width, i); cout << char(221);
    }

    int i = 15;
    GotoXY(WIDTH_CONSOLE + 25, i); cout << "L:\tSAVE GAME"; i++;
    GotoXY(WIDTH_CONSOLE + 25, i); cout << "P:\tPAUSE GAME"; i++;
    GotoXY(WIDTH_CONSOLE + 25, i); cout << "ESC:\tEXIT GAME"; i++;

    GotoXY(WIDTH_CONSOLE + 6, 15); cout << "MOVE KEY";
    GotoXY(WIDTH_CONSOLE + 10, 16); cout << "W";
    GotoXY(WIDTH_CONSOLE + 9, 17); cout << "A";
    GotoXY(WIDTH_CONSOLE + 11, 17); cout << "D";
    GotoXY(WIDTH_CONSOLE + 10, 18); cout << "S";

    drawScore();
    //GotoXY(curPosX, curPosY);
}

void ExitGame() {
    system("cls");
    cout << "GOOD BYE!\n\n";
    system("pause");
}

void PauseGame() {
    GotoXY(0, HEIGH_CONSOLE + 2);
    cout << "Press any key to coninue!";
    while (!_kbhit());
}

//Hàm cập nhật dữ liệu toàn cục
void Eat1() {
    if (FOOD_INDEX == MAX_SIZE_FOOD - 1)
    {
        if (SPEED != MAX_SPEED)
        {
            GenerateFood();
            FOOD_INDEX = -1;
            SPEED++;
        };
    }
}

void throughGate()
{
    while (SIZE_SNAKE > 1)
    {
        for (int i = 1; i < SIZE_SNAKE; i++)
        {
            snake[i - 1] = snake[i];
        }
        snake[SIZE_SNAKE - 1] = { NULL, NULL };
        drawSnake();
        Sleep(400 - 100 * SPEED);
        clearSnake();
        SIZE_SNAKE--;
    }
    snake[SIZE_SNAKE - 1] = { NULL, NULL };
}

void Eat2()
{
    if (food_ate == MAX_SPEED * MAX_SIZE_FOOD)
    {
        GotoXY(food[FOOD_INDEX].x, food[FOOD_INDEX].y);
        printf(" ");
        drawGate();
        if (snake[SIZE_SNAKE - 1] == gate[0])
        {
            throughGate();
            SIZE_SNAKE = 6;
            snake[0] = { 10, 5 }; snake[1] = { 11, 5 };
            snake[2] = { 12, 5 }; snake[3] = { 13, 5 };
            snake[4] = { 14, 5 }; snake[5] = { 15, 5 };
            clearGate();
            clearRock();
            SPEED = 1;
            FOOD_INDEX = 0;
            level++;
            food_ate = 0;
            GenerateGate();
            createRock();
            GenerateFood();
        }
    }
}

void saveScore() {

    cout << "Enter your name: ";

    string s; cin >> s;

    ofstream out;
    out.open("ScoreBoard.txt", ios::out | ios::app);

    out << score << " " << s << endl;

    out.close();

}

void ProcessDead() {
    effectDead();
    STATE = 0;
    GotoXY(0, HEIGH_CONSOLE + 2);

    int k;
    cout << "DEAD!\n\nDo you want to save score:\n1. YES\n2. NO\n\n";
    cin >> k;

    system("cls");

    if (k == 1) saveScore();

    printf("Type y to new game or anykey to exit");
}

//Hàm vẽ màn hình
void clearSnake() {

    for (int i = 0; i < SIZE_SNAKE; i++) {
        GotoXY(snake[i].x, snake[i].y);
        printf(" ");
    }
}

void clearRock() {
    for (int i = 0; i < min(level, 5) * 3; i++) {
        GotoXY(rock[i].x, rock[i].y); printf("      ");
    }
}

//Thủ tục cho thread
void ThreadFunc() {
    while (!_kbhit() && STATE == 1) {
        clearSnake();

        if (MOVING != CHAR_LOCK) switch (MOVING) {

        case 'A':   Move(step[0]);
            break;

        case 'D':   Move(step[1]);
            break;

        case 'W':   Move(step[2]);
            break;

        case 'S':   Move(step[3]);
            break;
        }


        if (STATE == 1)
        {
            drawSnake();
            if (food_ate != MAX_SPEED * MAX_SIZE_FOOD)drawFood();
        }

        Sleep(400 - SPEED * 100); //Hàm ngủ theo tốc độ SPEEED
    }
}


//Hàm main
int main() {
    srand(time(NULL));
    int temp;
    ShowCur(0);
    FixConsoleWindow();
    //int randColor = rand() % (20 - 1 + 1) + 1;
    SetColor(30);
    startGame();

    while (1) {

        ThreadFunc();

        temp = toupper(_getch());

        if (STATE == 1) {
            if (temp == 'P') {
                PauseGame();
            }
            else if (temp == 27) {
                ExitGame();
                return 0;
            }
            if (temp == 'L') {
                saveGame();
                ExitGame();
                return 0;
            }
            else {
                if ((temp != CHAR_LOCK) && (temp == 'D' || temp == 'A' || temp == 'W' || temp == 'S'))
                {
                    if (temp == 'D') CHAR_LOCK = 'A';
                    else if (temp == 'W') CHAR_LOCK = 'S';
                    else if (temp == 'S') CHAR_LOCK = 'W';
                    else CHAR_LOCK = 'D';
                    MOVING = temp;
                }
            }
        }
        else {
            if (temp == 'Y') startGame();
            else
                return ExitGame(), 0;
        }
    }
}

