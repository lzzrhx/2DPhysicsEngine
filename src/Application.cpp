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

    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50.0, 0.0);
    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0 - 25.0, 0.0);
    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0 - 25.0, 0.0);
    floor->restitution = 0.5;
    leftWall->restitution = 0.2;
    rightWall->restitution = 0.2;
    floor->friction = 0.2;
    leftWall->friction = 0.2;
    rightWall->friction = 0.2;
    bodies.push_back(floor);
    bodies.push_back(leftWall);
    bodies.push_back(rightWall);

    Body* bigBox = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    //Body* bigBall = new Body(CircleShape(200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    bigBox->rotation = 1.4;
    bigBox->restitution = 0.7;
    bodies.push_back(bigBox);

    //Body* ball = new Body(CircleShape(50), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    //ball->restitution = 0.1;
    //bodies.push_back(ball);
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////


void Application::Input() {
    std::vector<Vec2> vertices = { Vec2(0.5, -0.25) * PIXELS_PER_METER, Vec2(0, 0.5) * PIXELS_PER_METER, Vec2(-0.5, -0.25) * PIXELS_PER_METER };
    // vertices = {
    //     Vec2(20, 50),
    //     Vec2(-40, 20),
    //     Vec2(-20, -60),
    //     Vec2(20, -60),
    //     Vec2(40, 20)
    // };

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEMOTION:
                // int x;
                // int y;
                // SDL_GetMouseState(&x, &y);
                // bodies[1]->position = Vec2(x, y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x;
                int y;
                SDL_GetMouseState(&x, &y);
                //Body* ball;
                //box = new Body(BoxShape(50, 50), x, y, 1.0);
                //ball = new Body(CircleShape(50), x, y, 1.0);
                //ball->restitution = 0.5;
                //ball->friction = 0.4;
                //bodies.push_back(ball);
                Body* polygon;
                polygon = new Body(PolygonShape(vertices), x, y, 2.0);
                polygon->restitution = 0.2;
                polygon->friction = 0.7;
                bodies.push_back(polygon);
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

    // Apply forces to bodies
    for(auto body: bodies) {
        Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
        body->AddForce(weight);
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
                contact.ResolveCollision();
                if (debug) {
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                    Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15.0, contact.start.y + contact.normal.y * 15.0, 0xFFFF00FF);
                }
                a->isColliding = true;
                b->isColliding = true;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    for (auto body: bodies) {
        Uint32 color = body->isColliding && debug ? 0xFF0000FF : 0xFFFFFFFF;
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
        }
        else if (body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
        }
        else if (body->shape->GetType() == POLYGON) {
            PolygonShape* polygonShape = (PolygonShape*) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, color);
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
