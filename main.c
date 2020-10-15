#define GLFW_NO_INCLUDE
#include <GLFW/glfw3.h>
#include <vinox/vinox.h>
#include <vinox/camera.h>
#include <vinox/texture.h>
#include <vinox/sound.h>
#include <vinox/buffer.h>
#include <stdbool.h>
#include <stdio.h>

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

#define PLAYERCOLOR (Vector4) { 0.0f, 1.0f, 0.0f, 1.0f }
#define RED (Vector4) { 1.0f, 0.0f, 0.0f, 1.0f }
#define EMPTYQUAD (Quad) { { 0.0f, 0.0f }, { 0.0f, 0.0f } }

/* Global variables in file */
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static Quad player;

int isKeyPressed(GLFWwindow *window, int keycode) {
    int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

void processInput(GLFWwindow *window) {
    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    const float playerSpeed = 150.0f * deltaTime;
    if (isKeyPressed(window, GLFW_KEY_W)) {
        player.position.y -= playerSpeed;
    }
    if (isKeyPressed(window, GLFW_KEY_S)) {
        player.position.y += playerSpeed;
    }
    if (isKeyPressed(window, GLFW_KEY_A)) {
        player.position.x -= playerSpeed;
    }
    if (isKeyPressed(window, GLFW_KEY_D)) {
        player.position.x += playerSpeed;
    }
}

int main(void) {
    GLFWwindow* window;
    glfwInit();
    
    window = glfwCreateWindow(1280, 720, "Vinox", NULL, NULL);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (vinoxInit(1280, 720) == -1) {
        printf("Init of vinox failed. aborting\n");
        return -1;
    }

    Camera camera;
    camera.scale = 1.0f;
    camera.rotation = 0.0f;

    Camera camera2;
    camera2.scale = 1.0f;
    camera2.rotation = 0.0f;
    camera2.origin = (Vector2) { 0.0f, 0.0f };
    camera2.position = (Vector2) { 10.0f, 10.0f };

    FrameBuffer renderTexture;
    renderTexture.texture.width = 640;
    renderTexture.texture.height = 480;
    renderTexture.scaleType = GL_LINEAR;
    FrameBuffer renderTexture2;
    renderTexture2.texture.width = 1280;
    renderTexture2.texture.height = 720;
    renderTexture2.scaleType = GL_NEAREST;
    Texture containerTex;
    Texture dinosaur;
    Texture smileTex;
    vinoxLoadTexture("dinosaur.png", &dinosaur, GL_NEAREST);
    vinoxLoadTexture("test.jpg", &containerTex, GL_LINEAR);
    vinoxLoadTexture("awesomeface.png", &smileTex, GL_NEAREST);
    
    Texture smileTex1;
    Texture smileTex2;
    Texture smileTex3;
    Texture smileTex4;
    Texture smileTex5;
    vinoxLoadTexture("test.jpg", &smileTex1, GL_LINEAR);
    vinoxLoadTexture("test.jpg", &smileTex4, GL_LINEAR);
    vinoxLoadTexture("test.jpg", &smileTex5, GL_LINEAR);
    vinoxLoadTexture("test.jpg", &smileTex2, GL_LINEAR);
    vinoxLoadTexture("test.jpg", &smileTex3, GL_LINEAR);

    vinoxCreateFramebuffer(&renderTexture);
    vinoxCreateFramebuffer(&renderTexture2);
    vinoxBeginTexture(&renderTexture);
        vinoxClear((Vector4) { 0.2, 0.2, 0.2, 1.0});
        vinoxBeginCamera(&camera2);
        vinoxCreateQuad((Quad) { { 160, 120 }, { 320, 240 } }, EMPTYQUAD, smileTex.id, RED, 180.0f);
        vinoxCreateQuad((Quad) { { 480, 120 }, { 320, 240 } }, EMPTYQUAD, 0, WHITE, 180.0f);
        vinoxCreateQuad((Quad) { { 480, 360 }, { 320, 240 } }, EMPTYQUAD, dinosaur.id, RED, 180.0f);
        vinoxCreateQuad((Quad) { { 160, 360 }, { 320, 240 } }, EMPTYQUAD, 0, WHITE, 180.0f);
        vinoxEndCamera();
    vinoxEndTexture(&renderTexture);
    player.size.x = 50.0f;
    player.size.y = 50.0f;
    double previousTime = glfwGetTime();
    int frameCount = 0;
    while (!glfwWindowShouldClose(window)) {
        int width, height;

        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - previousTime >= 1.0) {
            printf("Frames FPS: %i\n", frameCount);
            printf("Mili second time: %f\n", 1000.0/(double)(frameCount));

            frameCount = 0;
            previousTime = currentTime;
        }

        processInput(window);
    
        if (player.position.x < 0)
            player.position.x = 0;

        if (player.position.x > 1000)
            player.position.x = 1000;
        
        if (player.position.y < 0)
            player.position.y = 0;
        
        if (player.position.y > 1000)
            player.position.y = 1000;

        glfwGetFramebufferSize(window, &width, &height);
        float scaleX = (float)width/1280;
        float scaleY = (float)height/720;
        double posx, posy;

        glfwGetCursorPos(window, &posx, &posy);

        Vector2 screenSize = (Vector2) { width, height };
        Vector2 mousePos = (Vector2) { (int)posx/scaleX, (int)posy/scaleY };
        Vector2 screenMouse = vinoxScreenToWorld(mousePos, &camera);
        //printf("Posx: %i\n", (int)posx);
        //printf("Posy: %i\n", (int)posy);
        //printf("Player position: %i, %i\n", (int)player.position.x, (int)player.position.y);
        //printf("Mouse pos: %i, %i\n", (int)screenMouse.x, (int)screenMouse.y);

        camera.position = player.position;
        camera.origin = (Vector2) { 1280/2, 720/2 };

        vinoxBeginDrawing(width, height);
           vinoxClear((Vector4){ 0.0f, 0.0f, 0.0f, 1.0f });
            vinoxBeginTexture(&renderTexture2);
            vinoxBeginCamera(&camera);    
            vinoxClear((Vector4) { 0.2f, 0.2f, 0.2f, 1.0f });
            for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 20; x++) {
                int id = 0;
                if ((x + y) % 2 == 0)
                    id = containerTex.id;
                else if ((x + y) % 2 == 1)
                    id = smileTex.id;
                
                vinoxCreateQuad((Quad) { { x * 50, y * 50 }, { 50.0f, 50.0f } }, EMPTYQUAD, id, WHITE, 180.0f);
            }
        }
                vinoxCreateQuad((Quad) { { 320.0f, 640.0f }, { 640, 480 } }, EMPTYQUAD, renderTexture.texture.id, WHITE, 0.0f);
                vinoxCreateQuad(player, (Quad) { 0.0f, 0.0f, dinosaur.width, dinosaur.height }, dinosaur.id, WHITE, 0.0f);
                vinoxCreateQuad((Quad) { screenMouse, { 50.0f, 50.0f } }, EMPTYQUAD, 0, PURPLE, 0.0f);
                vinoxCreateQuad((Quad) { { 10.0f, 10.0f }, { 50.0f, 50.0f } }, EMPTYQUAD, smileTex1.id, PURPLE, 0.0f);
                vinoxCreateQuad((Quad) { { 60.0f, 60.0f }, { 50.0f, 50.0f } }, EMPTYQUAD, smileTex2.id, PURPLE, 0.0f);
                vinoxCreateQuad((Quad) { { 110.0f, 110.0f }, { 50.0f, 50.0f } }, EMPTYQUAD, smileTex3.id, PURPLE, 0.0f);
                vinoxCreateQuad((Quad) { { 160.0f, 160.0f }, { 50.0f, 50.0f } }, EMPTYQUAD, smileTex4.id, PURPLE, 0.0f);
                vinoxCreateQuad((Quad) { { 210.0f, 210.0f }, { 50.0f, 50.0f } }, EMPTYQUAD, smileTex5.id, PURPLE, 0.0f);
            vinoxEndCamera();
            vinoxEndTexture(&renderTexture2);
                vinoxCreateQuad((Quad) { { width/2, height/2 }, { width, -height } }, EMPTYQUAD, renderTexture2.texture.id, WHITE, 0.0f);
            vinoxCreateQuad((Quad) { { 0.0f, 0.0f }, { 100.0f, 100.0f}}, EMPTYQUAD, 0, WHITE, 0.0f);
        vinoxEndDrawing();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vinoxEnd();
    return 0;
}
