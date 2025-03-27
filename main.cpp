//Author Nilfred Baez

#include <raylib.h>

using namespace std;

const int screen_width = 1280;
const int screen_height = 800;
Color nightBlue     = {25, 25, 112, 255};      // Backgroud
Color electricLime  = {204, 255, 0, 255};      // Ball
Color cyanSky       = {0, 255, 255, 255};      // Paddle player
Color magentaPunch  = {255, 0, 255, 255};      // Paddle CPU
Color ivoryWhite    = {255, 255, 240, 255};    // Text
Color coolGray      = {160, 160, 160, 255};    // Central line
Color goldenrod     = {255, 215, 0, 255};      // Winner

int player_score = 0;
int cpu_score = 0;


//menu logic

enum GameState { MENU, PLAYING, GAMEOVER };
GameState gameState = MENU;

const int WIN_SCORE = 7;
bool playerWon = false;


//Classes

class Ball {
    public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw(Color color) {
        DrawCircle(x, y, 20, color);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        } 
        if(x + radius >= GetScreenWidth() ) //Cpu score
        {
            cpu_score++;
            ResetBall();

        }
        if(x - radius <= 0) //Player score
        {
            player_score++;
            ResetBall();
        }

        if(x + radius >= GetScreenWidth()        
        || x - radius <= 0) {
            ResetBall();
        }
    }

    //Reset ball after win
    void ResetBall(){
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        int speed_choices[2] = {-1,1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }

};

class Paddle {
    protected:

    void LimitMovement() {
        if(y <= 0) {
            y = 0;
        }
        if(y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }
    public:
    float x, y;
    int speed;
    float width, height;

    void Draw(Color color) {
        DrawRectangle(x, y, width, height, color);
    }

    void Update() {
        if(IsKeyDown(KEY_UP)) {
            y -= speed;
        }
        if(IsKeyDown(KEY_DOWN)) {
            y += speed;
        }

        LimitMovement();
    }

};

class CpuPaddle : public Paddle{
    public:
    void Update(int ball_y) {
        if(y + height / 2 > ball_y) {
            y = y  - speed;
        }
        if(y + height / 2 <= ball_y) {
            y = y + speed;
        }
        LimitMovement();
    }
    void Draw(Color color) {
        DrawRectangle(x, y, width, height, color);
    }


};

Ball ball;
Paddle player;
CpuPaddle cpu;



int main()
{
    

    InitWindow(screen_width, screen_height, "Retro Pong"); //Title, Windows Defs
    SetTargetFPS(60);
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (!WindowShouldClose()) { //Principal Bucle

        BeginDrawing();
        ClearBackground(nightBlue);
    
        if (gameState == MENU) {
            DrawText("Retro Pong", screen_width / 2 - 100, screen_height / 2 - 60, 40, ivoryWhite);
            DrawText("Press [SPACE] to sart", screen_width / 2 - 180, screen_height / 2, 20, ivoryWhite);
    
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = PLAYING;
                player_score = 0;
                cpu_score = 0;
                ball.ResetBall();
            }
    
        } else if (gameState == PLAYING) {
            // Updates
            ball.Update();
            player.Update();
            cpu.Update(ball.y);
    
            // Collitions
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
                ball.speed_x *= -1;
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
                ball.speed_x *= -1;
    
            // Draws
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, coolGray);
            ball.Draw(electricLime);
            cpu.Draw(magentaPunch);
            player.Draw(cyanSky);
    
            DrawText(TextFormat("%i", player_score), screen_width / 2 + 100, 50, 30, ivoryWhite);
            DrawText(TextFormat("%i", cpu_score), screen_width / 2 - 100, 50, 30, ivoryWhite);
    
            // Win Condition
            if (player_score >= WIN_SCORE || cpu_score >= WIN_SCORE) {
                playerWon = (player_score > cpu_score);
                gameState = GAMEOVER;
            }
    
        } else if (gameState == GAMEOVER) {
            DrawText(playerWon ? "You Won!" : "You Lost :c!", screen_width / 2 - 80, screen_height / 2 - 40, 40, goldenrod);
            DrawText("Press [R] To back menu", screen_width / 2 - 170, screen_height / 2 + 20, 20, ivoryWhite);
    
            if (IsKeyPressed(KEY_R)) {
                gameState = MENU;
            }
        }
    
        EndDrawing();
    }
    

    CloseWindow();

    return 0;
}