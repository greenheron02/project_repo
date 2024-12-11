#include <iostream>
#include "gamepad.h"

int main() {
    // Initialize SDL
    qDebug() << "AAAAA";
    if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Check if at least one controller is connected
    if (SDL_NumJoysticks() < 1) {
        std::cerr << "No joysticks or gamepads found!" << std::endl;
        SDL_Quit();
        return 1;
    }

    // Open the first available game controller
    SDL_GameController *controller = SDL_GameControllerOpen(0);
    if (controller == nullptr) {
        std::cerr << "Could not open game controller 0: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    // Event loop to handle controller input
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Check button states
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
            std::cout << "A button pressed!" << std::endl;
        }

        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B)) {
            std::cout << "B button pressed!" << std::endl;
        }

        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X)) {
            std::cout << "X button pressed!" << std::endl;
        }

        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y)) {
            std::cout << "Y button pressed!" << std::endl;
        }

        // Read axis states (for example, left stick)
        std::cout << "Left Stick X: " << SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) << std::endl;
        std::cout << "Left Stick Y: " << SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) << std::endl;

        // Adding a small delay for better responsiveness
        SDL_Delay(100);
    }

    // Clean up and close the controller
    SDL_GameControllerClose(controller);
    SDL_Quit();

    return 0;
}
