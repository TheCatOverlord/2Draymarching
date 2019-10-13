#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#define NUM_OF_RAYS 1080
#define MAX_STEPS 50
#define FRAMERATE 6000

typedef struct RayMarch
{
    Vector2 pos;
    float distance;
    float angle;
} RayMarch;

typedef struct Sphere
{
    Vector2 pos;
    float radius;
} Sphere;

typedef struct Plane
{
    float value;
    bool isX;
} Plane;

float GetDist(Vector2 pos);

int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 600;
    float frayCount = (float) NUM_OF_RAYS;

    InitWindow(screenWidth, screenHeight, "2D Ray Marching");

    RayMarch rays[NUM_OF_RAYS] = { 0 };
    Sphere ball = { 0 };
    Vector2 camera = (Vector2) { screenWidth/2, screenHeight/2 };
    Plane floor = { 0 };
    bool hit = false;
    bool drawCircle = false;
    bool drawPoints = false;
    bool help = false;
    bool zerothRay = false;
    bool lines = false;

    // Setup rays
    for (int i = 0; i < NUM_OF_RAYS; i++)
    {
        rays[i].pos = camera;
        rays[i].distance = 0.0f;
        rays[i].angle = (360.0f/frayCount) * i;
    }

    // Setup the Ball
    ball.pos = (Vector2) { screenWidth * 7/8, screenHeight/2 };
    ball.radius = 50;
    //Setup Floor
    floor.isX = true;
    floor.value = screenHeight - 20;

    SetTargetFPS(FRAMERATE);

    while(!WindowShouldClose())
    {
        // Update
        if(IsKeyDown(KEY_W)) camera.y--;
        if(IsKeyDown(KEY_S)) camera.y++;
        if(IsKeyDown(KEY_A)) camera.x--;
        if(IsKeyDown(KEY_D)) camera.x++;
        if(IsKeyPressed(KEY_R)) drawCircle = !drawCircle;
        if(IsKeyPressed(KEY_C)) drawPoints = !drawPoints;
        if(IsKeyPressed(KEY_H)) help = !help;
        if(IsKeyPressed(KEY_F)) zerothRay = !zerothRay;
        if(IsKeyPressed(KEY_E)) lines = !lines;
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) ball.pos = GetMousePosition();
        if(IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) camera = GetMousePosition();

        // Draw
        BeginDrawing();
            ClearBackground(BLACK);

            for (int i = 0; i < NUM_OF_RAYS; i++)
            {
                rays[i].distance = 0.0f;
                rays[i].pos = camera;
                hit = false;
                for (int j = 0; j < MAX_STEPS; j++)
                {
                    // Calculate floor distance
                    if (floor.isX)
                    { rays[i].distance = abs(floor.value - rays[i].pos.y); }
                    else if (!floor.isX)
                    { rays[i].distance = abs(floor.value - rays[i].pos.y); }

                    // Calculate ball distance and set if smaller
                    if ((sqrt(pow(rays[i].pos.x - ball.pos.x, 2) + pow(rays[i].pos.y - ball.pos.y, 2)) - ball.radius) < rays[i].distance)
                    { rays[i].distance = sqrt(pow(rays[i].pos.x - ball.pos.x, 2) + pow(rays[i].pos.y - ball.pos.y, 2)) - ball.radius; }

                    // Draw if hit anything
                    if(drawPoints) DrawPixel(rays[i].pos.x, rays[i].pos.y, GREEN);
                    if(drawCircle) DrawCircleLines(rays[i].pos.x, rays[i].pos.y, rays[i].distance, (Color) { 255,255,255,50 });

                    if(zerothRay) {
                        if(i == 0) {
                            DrawPixel(rays[i].pos.x, rays[i].pos.y, GREEN); 
                            DrawCircleLines(rays[i].pos.x, rays[i].pos.y, rays[i].distance, (Color) { 255,255,255,50 }); }}

                    if (rays[i].distance < 0.1f)
                    {
                        DrawPixel(rays[i].pos.x, rays[i].pos.y, RED);
                        j = MAX_STEPS + 1;
                    }
                    // Add to pos 
                    rays[i].pos.x = (cos((PI/180) * rays[i].angle) * rays[i].distance) + rays[i].pos.x;
                    rays[i].pos.y = (sin((PI/180) * rays[i].angle) * rays[i].distance) + rays[i].pos.y;
                }
                if(lines) { DrawLine(rays[i].pos.x, rays[i].pos.y, camera.x, camera.y, WHITE); }
            }
            if(help)
            {
                DrawText("WASD = Move Camera", 10, 40, 10, GREEN);
                DrawText("C = Draw points of each iteration", 10, 50, 10, GREEN);
                DrawText("R = Draw circle around each iteration of each ray", 10, 60, 10, GREEN);
                DrawText("H = Display this text", 10, 70, 10, GREEN);
                DrawText("F = Display circle and point of zeroth ray", 10, 80, 10, GREEN);
                DrawText("E = Draw rays as lines", 10, 90, 10, GREEN);
                DrawText("LMB = Move ball", 10, 100, 10, GREEN);
                DrawText("RMB = Move Camera", 10, 110, 10, GREEN);
            }
            DrawCircle(camera.x, camera.y, 2, PINK);
            DrawFPS(10,10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}