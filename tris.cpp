#include <iostream>

using std::cout, std::cin, std::string, std::stoi;

#define RED_BOLD "\033[31;1m"
#define BLUE_BOLD "\033[34;1m"
#define RESET "\033[0m"

const int ROWS = 3;
const int COLUMNS = 3;
const int TOTAL_CELLS = ROWS * COLUMNS;
char board[ROWS][COLUMNS];
const string COMPUTER_NAME = "Computer";
int matches;

struct Player
{
    string name;
    char move;
    int win;
};

Player p1{.move = 'X'};
Player p2{.move = 'O'};

int readInt();
bool startGame(char gameMode);
void namesPlayers();
void printBoard();
void playerMove(Player player);
void placeMarker(Player & player, int position);
bool computerMove();
bool checkWin(Player & player);
bool checkTie(int moveCount);
void resetBoard();
void showWins();
bool playAgain();

int main()
{
    int menuChoice;
    char gameMode;

    while (true)
    {   
        p1.win = 0;
        p2.win = 0;
        matches = 0;

        system("clear");
        
        cout << "---------TRIS---------\n";
        cout << "Choose an option:\n";
        cout << "1. Player vs Player\n";
        cout << "2. Player vs Computer\n";
        cout << "3. Exit\n";
        cout << "--> ";
        menuChoice = readInt();

        if (menuChoice == 3)
        {
            cout << "Exiting program...\n";
            break;
        }
        
        switch (menuChoice)
        {
            case 1:
                gameMode = 'p';
                p2.name = "";
                resetBoard();
                namesPlayers();
                break;

            case 2:
                gameMode = 'c';
                p2.name = COMPUTER_NAME;
                resetBoard();
                namesPlayers();
                break;

            default:
                continue;
        }

        while (startGame(gameMode))
        {
            resetBoard();
        }
    }

    return 0;
}

int readInt()
{
    string inputString;
    int inputNumber;

    cin >> inputString;

    try
    {
        inputNumber = stoi(inputString);
        return inputNumber;
    }
    catch (...)
    {
        return 0;
    }

}

bool startGame(char gameMode)
{
    int moveCount = 0;

    do
    {
        printBoard();
        showWins();
        moveCount++;

        if (matches % 2 == 0)
        {
            playerMove(p1);
            if (checkWin(p1))
                break;
        }
        else if (gameMode == 'p' && matches % 2 == 1)
        {
            playerMove(p2);
            if (checkWin(p2))
                break;
        }
        else
        {
            computerMove();
            if (checkWin(p2))
                break;
        }

        if (checkTie(moveCount))
            break;

        printBoard();
        showWins();
        moveCount++;

        if (matches % 2 == 1)
        {
            playerMove(p1);
            if (checkWin(p1))
                break;
        }
        else if (gameMode == 'p' && matches % 2 == 0)
        {
            playerMove(p2);
            if (checkWin(p2))
                break;
        }
        else
        {
            computerMove();
            if (checkWin(p2))
                break;
        }
    }
    while (true);

    matches++;

    return playAgain();
}

void namesPlayers()
{
    system("clear");
    
    if (p2.name == COMPUTER_NAME)
    {
        cout << "---------TRIS---------\n";
        cout << "Insert your name:\n";
        cout << "--> ";
        cin >> p1.name;
    }

    else
    {
        cout << "---------TRIS---------\n";
        cout << "Insert FIRST player's name:\n";
        cout << "--> ";
        cin >> p1.name;

        cout << "\nInsert SECOND player's name:\n";
        cout << "--> ";
        cin >> p2.name;
    }
}

void printBoard()
{
    system("clear");

    cout << "---------TRIS---------\n\n";

    for (int i = 0; i < ROWS; i++)
    {
        cout << "     ";
        for (int j = 0; j < COLUMNS; j++)
        {
            if (board[i][j] == 'X')
                cout << RED_BOLD << " " << board[i][j] << " " << RESET;
            else if (board[i][j] == 'O')
                cout << BLUE_BOLD << " " << board[i][j] << " " << RESET;
            else
                cout << " " << board[i][j] << " ";

            if (j < COLUMNS - 1)
                cout << "|";
        }

        cout << "\n";

        if (i < ROWS - 1)
        {
            cout << "     " << "-----------\n";
        }
    }

    cout << "\n";
}

void playerMove(Player player)
{
    int position;

    cout << player.name << "'s turn\n";
    cout << "Choose where to place your \"" << player.move << "\": ";
    position = readInt();

    placeMarker(player, position);
}

void placeMarker(Player & player, int position)
{
    int rowIndex = (position - 1) / COLUMNS;
    int colIndex = (position - 1) % COLUMNS;

    if (position >= 1 && position <= TOTAL_CELLS && board[rowIndex][colIndex] != 'X' && board[rowIndex][colIndex] != 'O')
    {
        board[rowIndex][colIndex] = player.move;
    }

    else
    {
        printBoard();
        
        if (position == 0)
            cout << "Invalid input.\nChoose a position between 1 and 9.\n";
        else if (position < 1 || position > 9)
            cout << "Not a valid position.\nChoose a position between 1 and 9.\n";
        else
            cout << "Position alredy occupied.\nChoose another position.\n";

        cout << "----------------------\n";
        playerMove(player);
    }
}

bool computerMove()
{
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLUMNS; j++) 
        {
            if (board[i][j] != 'X' && board[i][j] != 'O') 
            {
                char originalValue = board[i][j];
                board[i][j] = p2.move;

                if (checkWin(p2))
                {
                    p2.win--;
                    return true;
                }

                board[i][j] = originalValue;
            }
        }
    }

    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLUMNS; j++) 
        {
            if (board[i][j] != 'X' && board[i][j] != 'O') 
            {
                char originalValue = board[i][j];
                board[i][j] = p1.move;

                if (checkWin(p2)) 
                {
                    p2.win--;
                    board[i][j] = p2.move;
                    return true;
                }

                board[i][j] = originalValue;
            }
        }
    }

    int position;

    do 
    {
        position = rand() % TOTAL_CELLS + 1;
    } 
    while (board[(position - 1) / COLUMNS][(position - 1) % COLUMNS] == 'X' || board[(position - 1) / COLUMNS][(position - 1) % COLUMNS] == 'O');

    placeMarker(p2, position);

    return false;
}

bool checkWin(Player & player)
{
    for (int i = 0; i < ROWS; i++)
    {
        if ((board[i][0] == board[i][1] && board[i][0] == board[i][2]) ||
            (board[0][i] == board[1][i] && board[0][i] == board[2][i]))
        {
            printBoard();
            player.win++;
            showWins();
            cout << player.name << " won!!\n";
            return true;
        }
    }

    if ((board[0][0] == board[1][1] && board[0][0] == board[2][2]) ||
        (board[0][2] == board[1][1] && board[0][2] == board[2][0]))
    {
        printBoard();
        player.win++;
        showWins();
        cout << player.name << " won!!\n";
        return true;
    }

    return false;
}

bool checkTie(int moveCount)
{
    if (moveCount == TOTAL_CELLS)
    {
        printBoard();
        showWins();
        cout << "It's a tie.\n";
        return true;
    }

    return false;
}

void resetBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            board[i][j] = '1' + i * COLUMNS + j;
        }
    }
}

void showWins()
{
    cout << p1.name << "'s wins: " << p1.win << "\n";
    cout << p2.name << "'s wins: " << p2.win << "\n";
    cout << "----------------------\n";
}

bool playAgain()
{
    char option;
    
    do
    {
        cout << "Do you want to play again? (y/n): ";
        cin >> option;
    
        if (option == 'y' || option == 'Y')
        {
            return true;
        }
        else if (option == 'n' || option == 'N')
        {
            return false;
        }
        else
        {
            printBoard();
            showWins();
            cout << "Not a valid option.\n";
        }
    }
    while (true);
}