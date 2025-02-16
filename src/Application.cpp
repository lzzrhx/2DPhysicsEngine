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
    world = new World(-9.8);

    Body* a = new Body(CircleShape(30), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0f);
    Body* b = new Body(CircleShape(20), a->position.x - 100, a->position.y, 1.0f);
    world->AddBody(a);
    world->AddBody(b);
    JointConstraint* joint = new JointConstraint(a, b, a->position);
    world->AddConstraint(joint);
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
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* body;
                    body = new Body(CircleShape(30), x, y, 1.0);
                    body->SetTexture("./assets/basketball.png");
                    body->restitution = 0.9;
                    body->friction = 0.2;
                    world->AddBody(body);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* body;
                    body = new Body(BoxShape(60, 60), x, y, 1.0);
                    body->SetTexture("./assets/crate.png");
                    body->restitution = 0.2;
                    body->friction = 0.7;
                    world->AddBody(body);
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_d)
                    debug = !debug;
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    Graphics::ClearScreen(0xFF000000);
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

    world->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    for (auto body: world->GetBodies()) {
        Uint32 color = body->isColliding && debug ? 0xFF0000FF : 0xFFFFFFFF;
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) body->shape;
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2.0, circleShape->radius * 2.0, body->rotation, body->texture);
            } else {
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
            }
        }
        else if (body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*) body->shape;
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            } else {
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
            }
        }
        else if (body->shape->GetType() == POLYGON) {
            PolygonShape* polygonShape = (PolygonShape*) body->shape;
            if (!debug) {
                Graphics::DrawFillPolygon(body->position.x, body->position.y, polygonShape->worldVertices, color);
            } else {
                Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, color);
            }
        }
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete world;
    Graphics::CloseWindow();
}
