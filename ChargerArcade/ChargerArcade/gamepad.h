/*#include <SDL2/SDL.h>
#include <QDebug>

class GamepadHandler {
public:
    GamepadHandler() {
        if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
            qDebug() << "SDL could not initialize! SDL_Error: " << SDL_GetError();
        } else {
            if (SDL_NumJoysticks() < 1) {
                qDebug() << "No joystick connected!";
            } else {
                controller = SDL_GameControllerOpen(0); // Open the first available controller
                if (controller == nullptr) {
                    qDebug() << "SDL could not open game controller! SDL_Error: " << SDL_GetError();
                } else {
                    qDebug() << "Game controller opened!";
                }
            }
        }
    }

    ~GamepadHandler() {
        if (controller) {
            SDL_GameControllerClose(controller);
            controller = nullptr;
        }
        SDL_Quit();
    }

    void update() {
        if (controller) {
            SDL_GameControllerUpdate();

            // Read button states
            for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
                if (SDL_GameControllerGetButton(controller, static_cast<SDL_GameControllerButton>(i)) == 1) {
                    qDebug() << "Button" << i << "pressed";
                }
            }

            // Read axis states
            qDebug() << "Left Stick X:" << SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
            qDebug() << "Left Stick Y:" << SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);
            qDebug() << "Right Stick X:" << SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
            qDebug() << "Right Stick Y:" << SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
        }
    }

private:
    SDL_GameController* controller = nullptr;
};

/*int main() {
    qDebug() << "AAAAAAAAAAAAAAAA";
    GamepadHandler gamepadHandler;
    // Main loop
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        gamepadHandler.update();
        SDL_Delay(100); // Delay to reduce CPU usage
    }

    return 0;
}*/
