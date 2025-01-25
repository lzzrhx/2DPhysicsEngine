#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    //SDL_GetMouseState(&x, &y);
    // Particle* particle = new Particle(100, 100, 1.0);
    // particle->radius = 20;
    // particles.push_back(particle);

    Particle* smallPlanet = new Particle(200, 200, 1.0);
    smallPlanet->radius = 6;
    particles.push_back(smallPlanet);
    
    Particle* bigPlanet = new Particle(500, 500, 20.0);
    bigPlanet->radius = 20;
    particles.push_back(bigPlanet);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
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
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                // if (event.button.button == SDL_BUTTON_LEFT) {i
                //     int x, y;
                //     SDL_GetMouseState(&x, &y);
                //     Particle* particle = new Particle(x, y, 1.0);
                //     particle->radius = 4;
                //     particles.push_back(particle);
                // }
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                    particles[0]->velocity = impulseDirection * impulseMagnitude;
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
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

    // Apply wind force to particles
    // for(auto particle: particles) {
    //     Vec2 wind = Vec2(0.5 * PIXELS_PER_METER, 0.0);
    //     particle->AddForce(wind);
    // }
    //Particle* particleA = particles[0];
    //Particle* particleB = particles[1];
    //Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 0.00000000006674);
    Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 1000.0, 5.0, 100.0);
    particles[0]->AddForce(attraction);
    particles[1]->AddForce(-attraction);

    // Apply forces to particles
    for(auto particle: particles) {
        //Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
        //particle->AddForce(weight);
        
        particle->AddForce(pushForce);
        
        // Vec2 friction = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
        // particle->AddForce(friction);

        // if (particle->position.y >= liquid.y) {
        //     Vec2 drag = Force::GenerateDragForce(*particle, 0.04);
        //     particle->AddForce(drag);
        // }

        // Integrate the acceleration and velocity to estimate the position
        particle->Integrate(deltaTime);
        
        if (particle->position.x + particle->radius <= 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -1.0;
        } else if (particle->position.x + particle->radius >= Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -1.0f;
        }
        if (particle->position.y + particle->radius <= 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -1.0f;
        } else if (particle->position.y + particle->radius + 24 >= Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius - 24;
            particle->velocity.y *= -0.6f;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    if (leftMouseButtonDown) {
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF00FF00);
    }

    Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF6E3713);

    // for (auto particle: particles) {
    //     Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    // }
    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFAA3300);
    Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFFF);
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto particle: particles) {
        delete particle;
    }

    Graphics::CloseWindow();
}
