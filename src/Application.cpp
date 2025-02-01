#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    //Body* bigBall = new Body(CircleShape(100), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    Body* boxA = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    Body* boxB = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    boxA->rotation = 2.3;
    bodies.push_back(boxA);
    bodies.push_back(boxB);
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
                int x, y;
                SDL_GetMouseState(&x, &y);
                bodies[0]->position = Vec2(x, y);
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
            //     if (event.key.keysym.sym == SDLK_UP)
            //         pushForce.y = -50 * PIXELS_PER_METER;
            //     if (event.key.keysym.sym == SDLK_DOWN)
            //         pushForce.y = 50 * PIXELS_PER_METER;
            //     if (event.key.keysym.sym == SDLK_LEFT)
            //         pushForce.x = -50 * PIXELS_PER_METER;
            //     if (event.key.keysym.sym == SDLK_RIGHT)
            //         pushForce.x = 50 * PIXELS_PER_METER;
                break;
            // case SDL_KEYUP:
            //     if (event.key.keysym.sym == SDLK_UP)
            //         pushForce.y = 0;
            //     if (event.key.keysym.sym == SDLK_DOWN)
            //         pushForce.y = 0;
            //     if (event.key.keysym.sym == SDLK_LEFT)
            //         pushForce.x = 0;
            //     if (event.key.keysym.sym == SDLK_RIGHT)
            //         pushForce.x = 0;
            //     break;
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

    // Apply forces to bodies
    for(auto body: bodies) {
        // Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
        // body->AddForce(weight);
        //Vec2 drag = Force::GenerateDragForce(*body, 0.01);
        //body->AddForce(drag);
        //Vec2 wind = Vec2(20.0 * PIXELS_PER_METER, 0.0);
        //body->AddForce(wind);
        // body->AddForce(pushForce);
        body->Update(deltaTime);
    }

    for (auto& body: bodies) {
        body->isColliding = false;
    }

    // Check all the bodies for collision
    for (size_t i = 0; i < bodies.size() - 1; i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            Body* a = bodies[i];
            Body* b = bodies[j];
            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)) {
                //contact.ResolveCollision();
                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15.0, contact.start.y + contact.normal.y * 15.0, 0xFFFF00FF);
                a->isColliding = true;
                b->isColliding = true;
            }
        }
    }

    // Keep body within bounds
    // for(auto body: bodies) {
    //     if (body->shape->GetType() == CIRCLE) {
    //         CircleShape* circleShape = (CircleShape*) body->shape;
    //         if (body->position.x - circleShape->radius <= 0) {
    //             body->position.x = circleShape->radius;
    //             body->velocity.x *= -1.0;
    //         } else if (body->position.x + circleShape->radius >= Graphics::Width()) {
    //             body->position.x = Graphics::Width() - circleShape->radius;
    //             body->velocity.x *= -1.0f;
    //         }
    //         if (body->position.y - circleShape->radius <= 0) {
    //             body->position.y = circleShape->radius;
    //             body->velocity.y *= -1.0f;
    //         } else if (body->position.y + circleShape->radius + 24 >= Graphics::Height()) {
    //             body->position.y = Graphics::Height() - circleShape->radius - 24;
    //             body->velocity.y *= -0.6f;
    //         }
    //     }
    // }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {

    for (auto body: bodies) {
        Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
        }
        else if (body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
        }
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    for (auto body: bodies) {
        delete body;
    }

    Graphics::CloseWindow();
}
