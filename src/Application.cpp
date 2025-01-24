#include "Application.h"
#include "./Physics/Constants.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    particle = new Particle(50, 100, 1.0);
    particle->radius = 4;
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // Cap the framerate to target FPS and set delta time
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016) {
        deltaTime = 0.016;
    }
    timePreviousFrame = SDL_GetTicks();

    // Update the objects in the scene
    particle->acceleration = Vec2(0.0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);

    particle->Integrate(deltaTime);

    if (particle->position.x + particle->radius <= 0) {
        particle->position.x = particle->radius;
        particle->velocity.x *= -0.9;
        //particle->velocity.y *= 0.95f;
    } else if (particle->position.x + particle->radius >= Graphics::Width()) {
        particle->position.x = Graphics::Width() - particle->radius;
        particle->velocity.x *= -0.9f;
        //particle->velocity.y *= 0.95f;
    }
    if (particle->position.y + particle->radius <= 0) {
        particle->position.y = particle->radius;
        particle->velocity.y *= -1.1f;
        particle->velocity.x *= 1.1f;
    } else if (particle->position.y + particle->radius + 100 >= Graphics::Height()) {
        particle->position.y = Graphics::Height() - particle->radius - 100;
        particle->velocity.y *= -0.7f;
        particle->velocity.x *= 0.8f;
    }
    //if ((int)particle->position.y + particle->radius >= Graphics::Height()) {
    //    particle->velocity.y = std::fabs(particle->velocity.y) * -0.6f;
    //}
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete particle;

    Graphics::CloseWindow();
}
