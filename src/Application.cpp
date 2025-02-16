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

    // Create a physics world with gravity of -9.8 m/s2
    world = new World(-9.8);

    // Add ragdoll parts (rigid bodies)
    Body* bob = new Body(CircleShape(5), Graphics::Width() / 2.0, Graphics::Height() / 2.0 - 200, 0.0);
    Body* head = new Body(CircleShape(25), bob->position.x, bob->position.y + 70, 5.0);
    Body* torso = new Body(BoxShape(50, 100), head->position.x, head->position.y + 80, 3.0);
    Body* leftArm = new Body(BoxShape(15, 70), torso->position.x - 32, torso->position.y - 10, 1.0);
    Body* rightArm = new Body(BoxShape(15, 70), torso->position.x + 32, torso->position.y - 10, 1.0);
    Body* leftLeg = new Body(BoxShape(20, 90), torso->position.x - 20, torso->position.y + 97, 1.0);
    Body* rightLeg = new Body(BoxShape(20, 90), torso->position.x + 20, torso->position.y + 97, 1.0);
    world->AddBody(bob);
    world->AddBody(head);
    world->AddBody(torso);
    world->AddBody(leftArm);
    world->AddBody(rightArm);
    world->AddBody(leftLeg);
    world->AddBody(rightLeg);
    
    // Add joints between ragdoll parts (distance constraints with one anchor point)
    JointConstraint* string = new JointConstraint(bob, head, bob->position);
    JointConstraint* neck = new JointConstraint(head, torso, head->position + Vec2(0, 25));
    JointConstraint* leftShoulder = new JointConstraint(torso, leftArm, torso->position + Vec2(-28, -45));
    JointConstraint* rightShoulder = new JointConstraint(torso, rightArm, torso->position + Vec2(+28, -45));
    JointConstraint* leftHip = new JointConstraint(torso, leftLeg, torso->position + Vec2(-20, +50));
    JointConstraint* rightHip = new JointConstraint(torso, rightLeg, torso->position + Vec2(+20, +50));
    world->AddConstraint(string);
    world->AddConstraint(neck);
    world->AddConstraint(leftShoulder);
    world->AddConstraint(rightShoulder);
    world->AddConstraint(leftHip);
    world->AddConstraint(rightHip);

    // Add a floor and walls to contain objects objects
    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0 - 25, 0.0);
    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0 - 25, 0.0);
    floor->restitution = 0.7;
    leftWall->restitution = 0.2;
    rightWall->restitution = 0.2;
    world->AddBody(floor);
    world->AddBody(leftWall);
    world->AddBody(rightWall);
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

    Body* bob = world->GetBodies()[0];
    Body* head = world->GetBodies()[1];
    Graphics::DrawLine(bob->position.x, bob->position.y, head->position.x, head->position.y, 0xFF555555);

    // Draw all joints anchor points
    for (auto joint: world->GetConstraints()) {
        if (debug) {
            const Vec2 anchorPoint = joint->a->LocalSpaceToWorldSpace(joint->aPoint);
            Graphics::DrawFillCircle(anchorPoint.x, anchorPoint.y, 3, 0xFF0000FF);
        }
    }

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
