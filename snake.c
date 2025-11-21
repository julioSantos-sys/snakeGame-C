#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int drawGrid(int screenHeight, int screenWidth, int widthGrid, int heightGrid){
    for (int i=0; i<screenHeight; i+=widthGrid){
                DrawLine(0, i, screenWidth, i, GRAY); 
            }
            for (int i=0; i<screenWidth; i+=heightGrid){
                DrawLine(i, 0, i, screenHeight, GRAY); 
            }
}
int drawWall(int screenHeight, int screenWidth, int widthGrid, int heightGrid){
    for (int y=0; y<=screenHeight-heightGrid; y+=heightGrid){
        for (int x=0; x<=screenWidth-widthGrid; x+=widthGrid){
            if (((x==0) || (x==(screenWidth-widthGrid))) || ((y == 0) || (y == screenHeight - heightGrid))){
                DrawRectangle(x, y, widthGrid, heightGrid, GRAY);

            }
        }
    }
}

int wallColision(Vector2 posSnakeHead, int borderBlockHeight, int borderBlockWidth, int screenMaxHeight, int screenMaxWidth){
    if ((posSnakeHead.x <= 0)
    || (posSnakeHead.x >= (screenMaxWidth - borderBlockWidth))
    || (posSnakeHead.y <=0)
    || (posSnakeHead.y >= (screenMaxHeight - borderBlockHeight))){
        return 1;
    } else{
        return 0;
    }

}

Vector2 generateApple(int screenHeight, int screenWidth, int widthGrid, int heightGrid, Vector2 snakeBody[], int snakeSize){
    int x, y;
    int minX, minY, maxX, maxY;

    int onSnake;

    while (1){
        onSnake = 0;
        minX = widthGrid;
        maxX = (screenWidth - (widthGrid*2));

        x = minX + rand() % (maxX - minX + 1);

        minY = heightGrid;
        maxY = (screenHeight - (heightGrid*2));
        
        y = minY + rand() % (maxY - minY + 1);

        x = (x / widthGrid) * widthGrid;
        y = (y / heightGrid) * heightGrid;

        for (int i=0; i<snakeSize; i++){
            if ((snakeBody[i].x == x) && (snakeBody[i].y == y)){
                onSnake = 1;
            } 
            
        }
        if (onSnake == 0){
            break;
        }
    }

    Vector2 apple = {x, y};

    return apple;

}

int appleCollision(Vector2 posSnake, Vector2 posApple){
    if ((posSnake.x == posApple.x) && (posSnake.y == posApple.y)){
        return 1;
    } else{
        return 0;
    }

}

int snakeCollisionSnake(Vector2 snakeBody[], int snakeSize){
    for (int i=0; i<snakeSize; i++){
        if (i!=0){
            if ((snakeBody[0].x == snakeBody[i].x) && (snakeBody[0].y == snakeBody[i].y)){
                return 1;
            }
        }
    }
    return 0;
}

int drawSnakeBody(Vector2 body[], int snakeSize, Vector2 snakeBlockSize){
    for (int i=0; i<snakeSize; i++){
        if (i != 0){
            DrawRectangleV(body[i], snakeBlockSize, RED);
        } else{
            DrawRectangleV(body[i], snakeBlockSize, PURPLE);
        }
    }
}

int game(int maxWidth, int maxHeight, int fpsGame){
    const int heightGrid = 50;
    const int widthGrid = 50;

    int centerX = maxWidth  / 2;
    int centerY = maxHeight / 2;

    Vector2 snakeHead = {(centerX / widthGrid) * widthGrid, (centerY / heightGrid) * heightGrid};
    Vector2 snakeBody[100];
    Vector2 snakeCubeSize = {widthGrid, heightGrid};

    int snakeSize=1;

    snakeBody[0] = snakeHead;

    int dirX = 0;
    int dirY = 0;

    float timer=0;
    float interval = 0.15;

    Vector2 applePos;

    applePos = generateApple(maxHeight, maxWidth, widthGrid, heightGrid, snakeBody, snakeSize);

    int interfaceFontSize = 20;
    
    int points=0;

    int fpsWidth;

    int nextDirX = 0;
    int nextDirY = 0;

    while(!WindowShouldClose()){

        if (IsKeyPressed(KEY_UP) && dirY != 1) {
        nextDirX = 0;
        nextDirY = -1;
        }
        else if (IsKeyPressed(KEY_DOWN) && dirY != -1) {
            nextDirX = 0;
            nextDirY = 1;
        }
        else if (IsKeyPressed(KEY_RIGHT) && dirX != -1) {
            nextDirX = 1;
            nextDirY = 0;
        }
        else if (IsKeyPressed(KEY_LEFT) && dirX != 1) {
            nextDirX = -1;
            nextDirY = 0;
        }


        timer += GetFrameTime();

        if (timer >= interval){
            dirX = nextDirX;
            dirY = nextDirY;

            snakeHead.x += dirX * widthGrid;
            snakeHead.y += dirY *heightGrid;
            timer = 0;

            
            for (int i=snakeSize-1; i>0; i--){
                snakeBody[i].x = snakeBody[i-1].x;
                snakeBody[i].y = snakeBody[i-1].y;
            }

            snakeBody[0].x = snakeHead.x;
            snakeBody[0].y = snakeHead.y;

        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            drawSnakeBody(snakeBody, snakeSize, snakeCubeSize);            
            DrawRectangleV(applePos, snakeCubeSize, BLUE);

            drawGrid(maxHeight, maxWidth, widthGrid, heightGrid);
            drawWall(maxHeight, maxWidth, widthGrid, heightGrid);

            char fps[20];
            char score[100];
            sprintf(score, "Score: %d", points);

            sprintf(fps, "FPS: %d | MAX: %d", GetFPS(), fpsGame);

            fpsWidth = MeasureText(fps, interfaceFontSize);

            DrawText(fps, 1, 1, interfaceFontSize, GREEN);
            DrawText(score, fpsWidth+10, 1, interfaceFontSize, GREEN);

            if (wallColision(snakeHead, heightGrid, widthGrid, maxHeight, maxWidth)){
                return 1;
            }

            if (snakeCollisionSnake(snakeBody, snakeSize) == 1){
                return 1;
            }

            if (appleCollision(snakeHead, applePos) == 1){
                applePos = generateApple(maxHeight, maxWidth, widthGrid, heightGrid, snakeBody, snakeSize);
                
                snakeBody[snakeSize-1].x = snakeBody[-2].x;
                snakeBody[snakeSize-1].y = snakeBody[-2].y;

                snakeSize++;

                points++;
            }
            


        EndDrawing();
    }
}

int main(){

    srand(time(NULL));

    const int screenWidth = 900;
    const int screenHeight = 650;

    InitWindow(screenWidth, screenHeight, "Snake");

    int fpsTarget = 120;

    SetTargetFPS(fpsTarget);    

    char start[] = "Press Enter to play";
    int fontSize = 50;

    int textWidth = MeasureText(start, fontSize);

    while (!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(start, (screenWidth-textWidth)/2, (screenHeight)/2, fontSize, GREEN);
        EndDrawing();

        if (IsKeyDown(KEY_ENTER)){
            game(screenWidth, screenHeight, fpsTarget);
        }
    }




}
