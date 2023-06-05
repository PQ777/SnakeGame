#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

const int width = 40;
const int height = 20;
const int maxSegments = 100;

enum class Direction
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct Position
{
    int x;
    int y;
};

class Snake
{
private:
    Position segments[maxSegments];
    int numSegments;
    Direction dir;

public:
    Snake()
    {
        numSegments = 1;
        dir = Direction::STOP;
        segments[0].x = width / 2;
        segments[0].y = height / 2;
    }

    int GetX(int segmentIndex) const
    {
        return segments[segmentIndex].x;
    }

    int GetY(int segmentIndex) const
    {
        return segments[segmentIndex].y;
    }

    Direction GetDirection() const
    {
        return dir;
    }

    void SetDirection(Direction direction)
    {
        dir = direction;
    }

    int GetNumSegments() const
    {
        return numSegments;
    }

    void Move()
    {
        for (int i = numSegments - 1; i > 0; --i)
        {
            segments[i] = segments[i - 1];
        }

        switch (dir)
        {
        case Direction::LEFT:
            segments[0].x--;
            break;

        case Direction::RIGHT:
            segments[0].x++;
            break;

        case Direction::UP:
            segments[0].y--;
            break;

        case Direction::DOWN:
            segments[0].y++;
            break;

        default:
            break;
        }
    }

    void Grow()
    {
        if (numSegments < maxSegments)
        {
            numSegments++;
        }
    }
};

class Apple
{
private:
    Position position;

public:
    Apple()
    {
        position.x = rand() % width;
        position.y = rand() % height;
    }

    int GetX() const
    {
        return position.x;
    }

    int GetY() const
    {
        return position.y;
    }

    void Respawn()
    {
        position.x = rand() % width;
        position.y = rand() % height;
    }
};

class Game
{
private:
    bool gameOver;
    Snake snake;
    Apple apple;
    int score;

public:
    Game()
    {
        gameOver = false;

        score = 0;
    }

    bool IsGameOver() const
    {
        return gameOver;
    }

    int GetScore() const
    {
        return score;
    }

    void ProcessInput()
    {
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'a':
            case 'A':
                snake.SetDirection(Direction::LEFT);
                break;

            case 'd':
            case 'D':
                snake.SetDirection(Direction::RIGHT);
                break;

            case 'w':
            case 'W':
                snake.SetDirection(Direction::UP);
                break;

            case 's':
            case 'S':
                snake.SetDirection(Direction::DOWN);
                break;

            case 'x':
            case 'X':
                gameOver = true;
                break;
            }
        }
    }

    void Update()
    {
        snake.Move();
        CheckCollision();
    }

    void CheckCollision()
    {
        int snakeX = snake.GetX(0);
        int snakeY = snake.GetY(0);

        // Check collision with snake's body segments
        for (int i = 1; i < snake.GetNumSegments(); i++)
        {
            if (snakeX == snake.GetX(i) && snakeY == snake.GetY(i))
            {
                gameOver = true;
                return;
            }
        }

        int appleX = apple.GetX();
        int appleY = apple.GetY();

        if (snakeX == appleX && snakeY == appleY)
        {
            score++;
            snake.Grow();
            apple.Respawn();
        }

        if (snakeX >= width || snakeX < 0 || snakeY >= height || snakeY < 0)
        {
            gameOver = true;
        }
    }


    void Draw()
    {
        system("cls");

        for (int i = 0; i < width / 2 + 2; i++)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << "■";
        }
        cout << endl;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (j == 0)
                    cout << "■";

                bool isSnakeSegment = false;
                for (int k = 0; k < snake.GetNumSegments(); k++)
                {
                    if (i == snake.GetY(k) && j == snake.GetX(k))
                    {
                        isSnakeSegment = true;
                        break;
                    }
                }

                if (isSnakeSegment)
                {
                    cout << "O";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                }
                else if (i == apple.GetY() && j == apple.GetX())
                    cout << "@";
                else
                    cout << " ";

                if (j == width - 1)
                    cout << "■";
            }
            cout << endl;
        }

        for (int i = 0; i < width / 2 + 2; i++)
            cout << "■";
        cout << endl;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        cout << "Score: " << score << endl;
    }
};

int main()

{
    system("mode con:cols=60 lines=40");
    Game game;

    cout << "========== 게임을 시작합니다 =====================" << endl;
    cout << "1. Easy / 2. Normal / 3. Hard" << endl;
    cout << "난이도를 선택하세요: ";
    int input;
    cin >> input;

    while (!game.IsGameOver())
    {
        game.Draw();
        game.ProcessInput();
        game.Update();
    }

    cout << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
    cout << "Game Over" << endl;
    cout << "Score: " << game.GetScore() << endl;


    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
    cout << "1. 재시작 / 2. 종료" << endl;
    cout << "선택: ";
    cin >> input;

    if (input == 1)
    {
        // 재시작
        main();
    }

    system("pause");
    return 0;
}
