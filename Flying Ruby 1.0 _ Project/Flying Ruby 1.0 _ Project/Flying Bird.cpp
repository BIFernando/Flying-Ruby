/* 

________  __            __                            _______             __                 
|        \|  \          |  \                          |       \           |  \                
| $$$$$$$$| $$ __    __  \$$ _______    ______        | $$$$$$$\ __    __ | $$____   __    __ 
| $$__    | $$|  \  |  \|  \|       \  /      \       | $$__| $$|  \  |  \| $$    \ |  \  |  \
| $$  \   | $$| $$  | $$| $$| $$$$$$$\|  $$$$$$\      | $$    $$| $$  | $$| $$$$$$$\| $$  | $$
| $$$$$   | $$| $$  | $$| $$| $$  | $$| $$  | $$      | $$$$$$$ | $$  | $$| $$  | $$| $$  | $$
| $$      | $$| $$__/ $$| $$| $$  | $$| $$__| $$      | $$  | $$| $$__/ $$| $$__/ $$| $$__/ $$
| $$      | $$ \$$    $$| $$| $$  | $$ \$$    $$      | $$  | $$ \$$    $$| $$    $$ \$$    $$
 \$$       \$$ _\$$$$$$$ \$$ \$$   \$$ _\$$$$$$$       \$$   \$$  \$$$$$$  \$$$$$$$  _\$$$$$$$
              |  \__| $$              |  \__| $$                                    |  \__| $$
               \$$    $$               \$$    $$                                     \$$    $$
                \$$$$$$                 \$$$$$$                                       \$$$$$$ 
                
 
 .----.   _     _       .----.-..-. .-.  .--.  .-.       .-.-..---. .---.   .-..----..----..-----.
| }`-' _{ }_ _{ }_     } |__| ||  \{ | / {} \ } |       | } }} }}_} {-. \  | |} |__}| }`-'`-' '-'
| },-.{_   _|_   _}    } '_}| }| }\  {/  /\  \} '--.    | |-'| } \\ '-} {`-' }} '__}| },-.  } {
`----'  {_}   {_}      `--' `-'`-' `-'`-'  `-'`----'    `-'  `-'-' `---' `---'`----'`----'  `-'

 __  __ ___ _____    ______  _____   ______  _  _ __            _   _       _  __
 |  \/  |_ _|_   _|  / |___ \|___ /  / |___ \| || |\ \          | | | | ___ | |/ /
 | |\/| || |  | |   | |  __) | |_ \ / /  __) | || |_| |         | | | |/ _ \| ' /
 | |  | || |  | |   | | / __/ ___) / /  / __/|__   _| |         | |_| | (_) | . \
 |_|  |_|___| |_|   | ||_____|____/_/  |_____|  |_| | |  _____   \___/ \___/|_|\_\
                     \_\                           /_/  |_____|

Developed by :
1. Shanuka 
2. Sanadi
3. Sasi
4. Bevindu

 */


#include "raylib.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

//----------------------------------------------------------------------------------
// Game Constants
//----------------------------------------------------------------------------------
const int NUM_FRAMES = 18;  // Bird animation image frames
const int ANIM_SPEED = 4;   // Bird animation speed
const float PLAYER_SPEED = 4.0f; //Bird movement speed
const float PIPE_GAP = 150.0f; //Gap between top and bottom pipes
const float INITIAL_GAME_SPEED = 2.0f; //Game runing speed
const int LEVEL_1_WIN_SCORE = 30; // Level 1 winning score
const int LEVEL_2_WIN_SCORE = 50; // Level 2 winning score
const float PIPE_SPACING_L1 = 350.0f; //Space between two pipe sets for Level 1
const float PIPE_SPACING_L2 = 300.0f; //Space between two pipe sets for Level 2


//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
class Player {
public:
    Vector2 position;
    int frameCounter;
    int currentFrame;

    Player() {
        position = { 168, 300 }; // Bird's start position
        frameCounter = 0;
        currentFrame = 0;
    }

    // Bird's animation management
    void UpdateAnimation() {
        frameCounter++;
        if (frameCounter >= ANIM_SPEED) {
            frameCounter = 0;
            currentFrame++;
            if (currentFrame >= NUM_FRAMES) {
                currentFrame = 0;
            }
        }
    }
};

class Pipe {
public:
    Rectangle topRec;
    Rectangle bottomRec;
    float x;
    bool scored;

    Pipe(float startX, float bottomOfTopPipeY, Texture2D& pipeTexture) {
        float pipeWidth = (float)pipeTexture.width;
        float pipeHeight = (float)pipeTexture.height;
        x = startX;
        topRec = { x, bottomOfTopPipeY - pipeHeight, pipeWidth, pipeHeight };
        bottomRec = { x, bottomOfTopPipeY + PIPE_GAP, pipeWidth, pipeHeight };
        scored = false;
    }

    void Update(float speed) {
        x -= speed;
        topRec.x = x;
        bottomRec.x = x;
    }
};


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    bool Quit = false;

    // Initialization
    const int screenWidth = 400;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Flying Ruby");
    InitAudioDevice();

  // Game States: 
    // 0 = Start
    // 1 = Level 1 Playing
    // 2 = Game Over
    // 3 = About
    // 4 = Pause
    // 5 = Level 1 Complete
    // 6 = Level 2 Playing
    // 7 = Level 2 Complete (Game Win)

    int gameState = 0;
    int score = 0;
    int currentLevel;
    float gameSpeed = INITIAL_GAME_SPEED;
    Texture2D pipeTex;
    Texture2D groundTex;
    Player player;
    vector<Pipe> pipes;

    int scoreTracker[1] = { 0 };    // The array holds the high score
    int* ptrHighScore = scoreTracker; // The pointer points to it
    
    


    float bgScrollSpeed = 1.0f;
    float speedIncreaseTimer = 0.0f;
    const float speedIncreaseInterval = 5.0f; // Game speed increase time intervel (5 sec)
    const float speedAmount = 0.25f; // Game speed increase by ( x0.25) in after every intervel
    
   
    

    // Load Sources
    Texture2D playerAnimTexture = LoadTexture("images/player anim.png");
    Texture2D instructionTexture = LoadTexture("images/inst.png");
    Texture2D MusicTexture = LoadTexture("images/Music.png");
    Texture2D AboutTexture = LoadTexture("images/About.png");
    Texture2D PauseTexture = LoadTexture("images/Pause.png");
    Font gameFont = LoadFont("fonts/BaiJamjuree-Bold.ttf");
    Font gameFont1 = LoadFont("fonts/omega-pixel-biform.ttf");
    Sound scoreSfx = LoadSound("sounds/score.wav");
    Sound slapSfx = LoadSound("sounds/slap.wav");
    Music menu = LoadMusicStream("sounds/menu.mp3");

    //Level 1 Assets
    Texture2D pipeUpImg = LoadTexture("images/pipe_up.png");
    Texture2D playerImg = LoadTexture("images/player.png");
    Texture2D pipeDownImg = LoadTexture("images/pipe_down.png");
    Texture2D groundImg = LoadTexture("images/ground.png");
    Texture2D bgImg = LoadTexture("images/background.png");
    Music level1 = LoadMusicStream("sounds/level1.mp3");

    //Level 2 Assets
    Texture2D pipeUpImg2 = LoadTexture("images/pipe_up2.png");
    Texture2D pipeDownImg2 = LoadTexture("images/pipe_down2.png");
    Texture2D groundImg2 = LoadTexture("images/ground2.png");
    Texture2D bgImg2 = LoadTexture("images/background2.png");
    Music level2 = LoadMusicStream("sounds/level2.mp3");

    menu.looping = true;
    level1.looping = true;
    level2.looping = true;
    

    PlayMusicStream(menu); // Start menu music initially
    bool isMusicOn = true;

    float frameWidth = (float)playerAnimTexture.width / NUM_FRAMES;
    float bgScroll = 0.0f;
    float groundScroll = 0.0f;

    

    auto resetGame = [&]() {
        currentLevel = 1; // Always Reset to level 1
        player = Player();
        pipes.clear();
        pipes.emplace_back(600, GetRandomValue(100, 320), pipeUpImg);
        score = 0;
        gameSpeed = INITIAL_GAME_SPEED;
        bgScrollSpeed = 1.0f;
        speedIncreaseTimer = 0.0f;
        };

    resetGame();
    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------
    // Main game loop
    //--------------------------------------------------------------------------------------
   
    while (!WindowShouldClose() && !Quit)
    {
       

                             // i. Background Music ON/OFF Logic
        UpdateMusicStream(menu);
        UpdateMusicStream(level1);
        UpdateMusicStream(level2);
        
        if (IsKeyPressed(KEY_M))
        {
            isMusicOn = !isMusicOn; // Flip the boolean

            if (!isMusicOn)
            {
                //  pause all tracks
                PauseMusicStream(menu);
                PauseMusicStream(level1);
                PauseMusicStream(level2);
            }
            else
            {
                // When turning music ON, resume the correct track for the current state
                switch (gameState)
                {
                case 0: // Start Screen
                case 3: // About Screen
                    ResumeMusicStream(menu);
                    break;
                case 1: // Playing Level 1
                case 5: // Level 1 Complete
                    ResumeMusicStream(level1);
                    break;
                case 6: // Playing Level 2
                case 7: // Game Win
                    ResumeMusicStream(level2);
                    break;
                }
            }
        }
        // ii. Game player movement
        if (gameState == 1 || gameState == 6) {
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { player.position.y -= PLAYER_SPEED; }
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { player.position.y += PLAYER_SPEED; }
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) { player.position.x -= PLAYER_SPEED; }
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { player.position.x += PLAYER_SPEED; }
        }

        // iii.Bird Boundary Rectangle
        Rectangle playerRec = { player.position.x, player.position.y,  50, 50 };

        if (gameState == 0 || gameState == 1 || gameState == 6) {
            player.UpdateAnimation();
        }
        
        //__________________________________________________________________

            //  Go to Game Playing Start
            if (gameState == 0)
            {
             
                bgScroll -= bgScrollSpeed;
                if (bgScroll <= -bgImg.width) bgScroll = 0;
                groundScroll -= gameSpeed;
                if (groundScroll <= -groundImg.width) groundScroll = 0;

                if (IsKeyPressed(KEY_SPACE)) {
                    gameState = 1;
                    StopMusicStream(menu); // stop meny music
                    PlayMusicStream(level1); // start level 1 music
                }


                // iii. Go to About screen
                if (IsKeyPressed(KEY_I))
                {
                    gameState = 3;
                }
            }

            // Go to Pause screen
            else if (gameState == 1)
            {
                if (IsKeyPressed(KEY_P)) {
                    gameState = 4;
                }

                // Cheat code for pass LEVEL 1 ; ( Shift + 1) o_O
                if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_ONE)) {
                    gameState = 5;
                }


                //______Game Playing Screen Working________________________________________________________________________________________________

                           
                // i. Scrolling
                speedIncreaseTimer += GetFrameTime();
                if (speedIncreaseTimer >= speedIncreaseInterval) {
                    speedIncreaseTimer = 0.0f;
                    gameSpeed += speedAmount;
                    bgScrollSpeed += speedAmount / 2;
                }

                

                // ii. Collision Management
                        //a. Pipe Collision
                for (auto& pipe : pipes) {
                    pipe.Update(gameSpeed);
                    if (CheckCollisionRecs(playerRec, pipe.topRec) || CheckCollisionRecs(playerRec, pipe.bottomRec)) {
                        PlaySound(slapSfx);
                        //Highest score taker
                        if (score > *ptrHighScore) {
                            *ptrHighScore = score;
                        }

                        gameState = 2;
                    }
                }
                //b. Boundry Collision
                if (player.position.y < 0 || player.position.y > screenHeight - playerAnimTexture.height - groundImg.height ||
                    player.position.x < 0 || player.position.x > screenWidth - frameWidth) {
                    PlaySound(slapSfx);
                    //Highest score taker
                    if (score > *ptrHighScore) {
                        *ptrHighScore = score;
                    }

                    gameState = 2;
                }

              
                //iii. Pipes creating
                if (!pipes.empty() && pipes.back().x < screenWidth - PIPE_SPACING_L1) {
                    pipes.emplace_back(screenWidth, GetRandomValue(100, 320), pipeUpImg);
                }
                // Pipes Deleting Logic
                if (!pipes.empty() && pipes.front().x < -pipeUpImg.width) {
                    pipes.erase(pipes.begin());
                }


                // Getting Scores
                for (auto& pipe : pipes) {
                    if (!pipe.scored && pipe.x + pipeUpImg.width < player.position.x) {
                        score++;
                        pipe.scored = true;
                        PlaySound(scoreSfx);

                        //Scores for win Level 1
                        if (score == LEVEL_1_WIN_SCORE) {
                            gameState = 5;
                        }
                    }
                }
                //Background Scrolling
                bgScroll -= bgScrollSpeed;
                if (bgScroll <= -bgImg.width) bgScroll = 0;
                groundScroll -= gameSpeed;
                if (groundScroll <= -groundImg.width) groundScroll = 0;
            }

            // Return toggles

            else if (gameState == 2) // Game Over
            {
                if (IsKeyPressed(KEY_ENTER)) {
                    resetGame();
                    StopMusicStream(level1);
                    StopMusicStream(level2);
                    PlayMusicStream(level1);

                    gameState = 1; // Go back to level 1
                }

                if (IsKeyPressed(KEY_Q)) {
                    Quit = true; // Quit Game

                }
            }


            else if (gameState == 3) // About Screen
            {
                if (IsKeyPressed(KEY_I))
                {
                    gameState = 0; // Go back to Start screen
                }
            }


            else if (gameState == 4) //Game pause
            {
                if (IsKeyPressed(KEY_P)) {
                    gameState = 1; //Go back to Level 1 Playing Screen
                }
            }
            // v. Go to 2nd Level playing screen
            else if (gameState == 5)
            {
                if (IsKeyPressed(KEY_SPACE)) {
                    gameState = 6;
                    StopMusicStream(level1); // stop level 1 music
                    PlayMusicStream(level2); // and start level 2 music
                    currentLevel = 2;
                    player = Player();
                    pipes.clear();
                    pipes.emplace_back(600, GetRandomValue(100, 320), pipeUpImg2);
                    gameSpeed = INITIAL_GAME_SPEED + 2.0f; //
                }
            }

            else if (gameState == 6) // Playing Level 2
            {
                if (IsKeyPressed(KEY_P)) {
                    gameState = 4;
                }

            

                // Cheat code for pass LEVEL 2 ; ( Shift + 2) o_O
                if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_TWO)) {
                    gameState = 7;
                }

                speedIncreaseTimer += GetFrameTime();
                if (speedIncreaseTimer >= speedIncreaseInterval) {
                    speedIncreaseTimer = 0.0f;
                    gameSpeed += speedAmount;
                    bgScrollSpeed += speedAmount / 2.0f;
                }

                

                for (auto& pipe : pipes) {
                    pipe.Update(gameSpeed);
                    if (CheckCollisionRecs(playerRec, pipe.topRec) || CheckCollisionRecs(playerRec, pipe.bottomRec)) {
                        PlaySound(slapSfx);
                        if (score > *ptrHighScore) *ptrHighScore = score;
                        gameState = 2;
                    }
                }

                if (player.position.y < 0 || player.position.y > screenHeight - playerAnimTexture.height - groundImg2.height ||
                    player.position.x < 0 || player.position.x > screenWidth - frameWidth) {
                    PlaySound(slapSfx);
                    if (score > *ptrHighScore) *ptrHighScore = score;
                    gameState = 2;
                }



                if (!pipes.empty() && pipes.back().x < screenWidth - PIPE_SPACING_L2) {
                    pipes.emplace_back(screenWidth, GetRandomValue(100, 320), pipeUpImg);
                }
                // Pipes Deleting Logic
                if (!pipes.empty() && pipes.front().x < -pipeUpImg.width) {
                    pipes.erase(pipes.begin());
                }

                for (auto& pipe : pipes) {
                    if (!pipe.scored && pipe.x + pipeUpImg2.width < player.position.x) {
                        score++;
                        pipe.scored = true;
                        PlaySound(scoreSfx);

                        if (score == LEVEL_2_WIN_SCORE) {
                            gameState = 7; // Game WIN!
                        }
                    }
                }



                bgScroll -= bgScrollSpeed;
                if (bgScroll <= -bgImg2.width) bgScroll = 0;
                groundScroll -= gameSpeed;
                if (groundScroll <= -groundImg2.width) groundScroll = 0;
            }
            else if (gameState == 7) // Game Complete
            {
                if (IsKeyPressed(KEY_ENTER)) {
                    resetGame();
                    StopMusicStream(level2);
                    PlayMusicStream(menu);

                    gameState = 0; // Go back to start screen

                    if (IsKeyPressed(KEY_Q)) {
                        Quit = true; // Quit Game
                    }
                }

            }
        

                
        //----------------------------------------------------------------------------------        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();


        ClearBackground(SKYBLUE);

        // Draw background based on level
        Texture2D& currentBg = (currentLevel == 1) ? bgImg : bgImg2;
        DrawTexture(currentBg, bgScroll, 0, WHITE);
        DrawTexture(currentBg, bgScroll + currentBg.width, 0, WHITE);

        // Draw pipes based on level
        if (gameState == 1 || gameState == 6 || gameState == 4 || gameState == 2) {
            Texture2D& currentPipeUp = (currentLevel == 1) ? pipeUpImg : pipeUpImg2;
            Texture2D& currentPipeDown = (currentLevel == 1) ? pipeDownImg : pipeDownImg2;
            for (const auto& pipe : pipes) {
                DrawTexture(currentPipeDown, pipe.topRec.x, pipe.topRec.y, WHITE);
                DrawTexture(currentPipeUp, pipe.bottomRec.x, pipe.bottomRec.y, WHITE);
            }
        }

        // Draw ground based on level
        Texture2D& currentGround = (currentLevel == 1) ? groundImg : groundImg2;
        DrawTexture(currentGround, groundScroll, screenHeight - currentGround.height, WHITE);
        DrawTexture(currentGround, groundScroll + currentGround.width, screenHeight - currentGround.height, WHITE);

        // Draw player
        DrawTextureRec(playerAnimTexture, { player.currentFrame * frameWidth, 0, frameWidth, (float)playerAnimTexture.height }, player.position, WHITE);

        // Draw score
        if (gameState == 1 || gameState == 6 || gameState == 4 || gameState == 2) {
            DrawTextEx(gameFont, to_string(score).c_str(), {(screenWidth - MeasureText(to_string(score).c_str(), 60)) / 2.0f, 64}, 60, 1, {10, 40, 9, 255});
        }


        if (gameState == 1) {
            const char* L1_Win_Score_Text = "/30";
            Vector2 L1_Win_Score_Size = MeasureTextEx(gameFont, L1_Win_Score_Text, 30, 1);
            DrawTextEx(gameFont, L1_Win_Score_Text, { (screenWidth - L1_Win_Score_Size.x) / 2 + ((MeasureText(to_string(score).c_str(), 60)) / 2.0f) + 27, 80 }, 30, 1, GRAY);
        }
            if (gameState == 6) {
                const char* L2_Win_Score_Text = "/60";
                Vector2 L2_Win_Score_Size = MeasureTextEx(gameFont, L2_Win_Score_Text, 30, 1);
                DrawTextEx(gameFont, L2_Win_Score_Text, { (screenWidth - L2_Win_Score_Size.x) / 2 + ((MeasureText(to_string(score).c_str(), 60)) / 2.0f) + 27, 80 }, 30, 1, GRAY);
           
        }
        //-----------------------------State-Specific Screens-------------------------------
        // 
        // i. START Screen
        //----------------------------------------------------------------------------------
        if (gameState == 0) {
            // Draw the instruction 'W,A,S,D' image 
            int instPosX = (screenWidth - instructionTexture.width) / 2 ;
            int instPosY = (screenHeight - instructionTexture.height) / 2 + 150; 
            DrawTexture(instructionTexture, instPosX-100, instPosY, WHITE); // Instruction Keys Image
            DrawTexture(MusicTexture, instPosX+120 , instPosY+20 -25, WHITE); // Music on/off Key Image
            DrawTexture(AboutTexture, instPosX+123 , instPosY+120 -25, WHITE); // About Key Image
            DrawTexture(PauseTexture, instPosX + 123, instPosY + 70 -25, WHITE); // Pause Key Image


            // Draw the start text 
            const char* startText = "Press 'SPACE' to Start";
            Vector2 startSize = MeasureTextEx(gameFont, startText, 30, 1);
            int instPosY1 = (screenHeight - instructionTexture.height) / 2 - 10; 
            DrawTextEx(gameFont, startText, { (screenWidth - startSize.x) / 2, (float)instPosY1 }, 30, 1, GRAY);

            // Draw the Title 
            const char* startText1 = "Flying Ruby";
            Vector2 startSize1 = MeasureTextEx(gameFont, startText1, 80, 1);
            //int instPosY1 = (screenHeight - instructionTexture.height) / 2 - 50; // Move it up slightly
            DrawTextEx(gameFont, startText1, { (screenWidth - startSize1.x) / 2, (float)instPosY1 - 80 }, 80, 1, RED);
        }


        // ii. GAME OVER Screen
        //----------------------------------------------------------------------------------
        else if (gameState == 2) {
            // Draw a semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(RED, 0.5f));

            // Draw  texts
            const char* gameOverText = "GAME OVER";
            Vector2 gameOverSize = MeasureTextEx(gameFont, gameOverText, 60, 1);
            DrawTextEx(gameFont, gameOverText, { (screenWidth - gameOverSize.x) / 2, (screenHeight / 2) - 100 }, 60, 1, RED);
            
            const char* restartText = "Press 'Enter' to Restart";
            Vector2 restartSize = MeasureTextEx(gameFont, restartText, 30, 1);
            DrawTextEx(gameFont, restartText, { (screenWidth - restartSize.x) / 2, (screenHeight / 2) + 20 }, 30, 1, WHITE);

            const char* QuitText = "Press 'Q' to Quit Game";
            Vector2 QuitSize = MeasureTextEx(gameFont, QuitText, 20, 1);
            DrawTextEx(gameFont, QuitText, { (screenWidth - QuitSize.x) / 2, (screenHeight / 2) + 200 }, 20, 1, RED);

            // Draw Highest Score
            string highScoreText = "Highest Score: " + to_string(*ptrHighScore);
            Vector2 highScoreSize = MeasureTextEx(gameFont, highScoreText.c_str(), 30, 1);
            DrawTextEx(gameFont, highScoreText.c_str(), { (screenWidth - highScoreSize.x) / 2, (screenHeight / 2) - 40 }, 30, 1, GOLD);
            
           
        }


        // iii. About Screen
        //----------------------------------------------------------------------------------
        else if (gameState == 3)
        {
            // Draw a semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));

            // Draw texts
            const char* aboutTitle = "About This Project";
            Vector2 titleSize = MeasureTextEx(gameFont, aboutTitle, 40, 1);
            DrawTextEx(gameFont, aboutTitle, { (screenWidth - titleSize.x) / 2, 150 }, 40, 1, WHITE);


            DrawText("This is our final project of P.C. module. ", 50, 220, 16, LIGHTGRAY);
            DrawText("Created with C++ and the Raylib library.", 50, 240, 16, LIGHTGRAY);
            DrawText("Developed by :", (50), 280, 12, LIGHTGRAY);
            DrawText("Shanuka _ IM/2023/001", 50, 300, 20, LIGHTGRAY);
            DrawText("Sanadi _ IM/2023/002", 50, 330, 20, LIGHTGRAY);
            DrawText("Sasivarnasarma _ IM/2023/132", 50, 360, 20, LIGHTGRAY);
            DrawText("Bevindu _ IM/2023/135", 50, 390, 20, LIGHTGRAY);
            DrawText("Project Date : July 2025", 25, 565, 12, GRAY);


            const char* returnText = "Press 'I' to Return";
            Vector2 returnSize = MeasureTextEx(gameFont, returnText, 30, 1);
            DrawTextEx(gameFont, returnText, { (screenWidth - returnSize.x) / 2, 450 +50 }, 30, 1, YELLOW);

            const char* UniText = "MIT(23/24) _ UoK";
            Vector2 UniSize = MeasureTextEx(gameFont, UniText, 24, 1);
            DrawTextEx(gameFont, UniText, { (screenWidth - UniSize.x) / 2, 450 -10 }, 24, 1, GRAY);
        }

        // iv. PAUSE Screen
        //----------------------------------------------------------------------------------
        else if (gameState == 4)
        {
            // Draw a semi-transparent overlay
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));

            // Draw texts
            const char* pausedText = "PAUSED";
            Vector2 pausedSize = MeasureTextEx(gameFont, pausedText, 60, 1);
            DrawTextEx(gameFont, pausedText, { (screenWidth - pausedSize.x) / 2, (screenHeight / 2) - 80 }, 60, 1, WHITE);

            const char* resumeText = "Press 'P' to Resume";
            Vector2 resumeSize = MeasureTextEx(gameFont, resumeText, 30, 1);
            DrawTextEx(gameFont, resumeText, { (screenWidth - resumeSize.x) / 2, (screenHeight / 2) + 20 }, 30, 1, LIGHTGRAY);
        }

        // v.  Level 1 Complete Screen
       //----------------------------------------------------------------------------------
        else if (gameState == 5) 
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GREEN, 0.5f));

            const char* Win1Text = "You Win Level 1!";
            Vector2 Win1Size = MeasureTextEx(gameFont, Win1Text, 60, 1);
            DrawTextEx(gameFont, Win1Text, { (screenWidth - Win1Size.x) / 2, (screenHeight / 2) - 80 }, 60, 1, GOLD);

            const char* Win1Text1 = "Press 'Space' for Level 2";
            Vector2 Win1Size1 = MeasureTextEx(gameFont, Win1Text1, 30, 1);
            DrawTextEx(gameFont, Win1Text1, { (screenWidth - Win1Size1.x) / 2, (screenHeight / 2)  }, 30, 1, WHITE);
        }

        // vi. Game Complete Screen
       //----------------------------------------------------------------------------------
        else if (gameState == 7) 
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(GREEN, 0.5f));

            const char* Win2Text = "You Win!";
            Vector2 Win2Size = MeasureTextEx(gameFont, Win2Text, 80, 1);
            DrawTextEx(gameFont, Win2Text, { (screenWidth - Win2Size.x) / 2, (screenHeight / 2) - 80 }, 80, 1, GOLD);

            const char* Win2Text1 = "Press 'Enter' to return to menu";
            Vector2 Win2Size1 = MeasureTextEx(gameFont, Win2Text1, 30, 1);
            DrawTextEx(gameFont, Win2Text1, { (screenWidth - Win2Size1.x) / 2, (screenHeight / 2) +40 }, 30, 1, WHITE);

            const char* Win2Text2 = "Level 3 Coming Soon...";
            Vector2 Win2Size2 = MeasureTextEx(gameFont, Win2Text2, 20, 1);
            DrawTextEx(gameFont, Win2Text2, { (screenWidth - Win2Size2.x) / 2, (screenHeight / 2)  }, 20, 1, BLACK);

            const char* QuitText = "Press 'Q' to Quit Game";
            Vector2 QuitSize = MeasureTextEx(gameFont, QuitText, 20, 1);
            DrawTextEx(gameFont, QuitText, { (screenWidth - QuitSize.x) / 2, (screenHeight / 2) + 200 }, 20, 1, RED);

        }
        EndDrawing(); 
    }

    // Unload
    //--------------------------------------------------------------------------------------
    UnloadTexture(playerAnimTexture);
    UnloadTexture(pipeUpImg);
    UnloadTexture(pipeDownImg);
    UnloadTexture(groundImg);
    UnloadTexture(bgImg);
    UnloadTexture(instructionTexture); 
    UnloadTexture(MusicTexture);
    UnloadTexture(AboutTexture);
    UnloadTexture(PauseTexture);
    UnloadFont(gameFont);
    UnloadSound(slapSfx);
    UnloadSound(scoreSfx);
    UnloadTexture(pipeUpImg2);
    UnloadTexture(pipeDownImg2);
    UnloadTexture(groundImg2);
    UnloadTexture(bgImg2);
    UnloadMusicStream(menu);
    UnloadMusicStream(level1);
    UnloadMusicStream(level2);

    CloseAudioDevice();
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}