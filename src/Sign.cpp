#include "Sign.h"

Sign::Sign(float x, float y)
    : InteractableObject(x,y, 32.0f,32.0f) {

}

void Sign::Interact() {
    SDL_Log("Sign interacted with.");
}