#include "Application.h"
#include "./Physics/Constants.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
// void Application::Setup() {
//     running = Graphics::OpenWindow();

//     // Create a physics world with gravity of -9.8 m/s2
//     world = new World(-9.8);

//     // Add ragdoll parts (rigid bodies)
//     // Body* bob = new Body(CircleShape(5), Graphics::Width() / 2.0, Graphics::Height() / 2.0 - 200, 0.0);
//     // Body* head = new Body(CircleShape(25), bob->position.x, bob->position.y + 70, 5.0);
//     // Body* torso = new Body(BoxShape(50, 100), head->position.x, head->position.y + 80, 3.0);
//     // Body* leftArm = new Body(BoxShape(15, 70), torso->position.x - 32, torso->position.y - 10, 1.0);
//     // Body* rightArm = new Body(BoxShape(15, 70), torso->position.x + 32, torso->position.y - 10, 1.0);
//     // Body* leftLeg = new Body(BoxShape(20, 90), torso->position.x - 20, torso->position.y + 97, 1.0);
//     // Body* rightLeg = new Body(BoxShape(20, 90), torso->position.x + 20, torso->position.y + 97, 1.0);
//     // world->AddBody(bob);
//     // world->AddBody(head);
//     // world->AddBody(torso);
//     // world->AddBody(leftArm);
//     // world->AddBody(rightArm);
//     // world->AddBody(leftLeg);
//     // world->AddBody(rightLeg);
    
//     // Add joints between ragdoll parts (distance constraints with one anchor point)
//     // JointConstraint* string = new JointConstraint(bob, head, bob->position);
//     // JointConstraint* neck = new JointConstraint(head, torso, head->position + Vec2(0, 25));
//     // JointConstraint* leftShoulder = new JointConstraint(torso, leftArm, torso->position + Vec2(-28, -45));
//     // JointConstraint* rightShoulder = new JointConstraint(torso, rightArm, torso->position + Vec2(+28, -45));
//     // JointConstraint* leftHip = new JointConstraint(torso, leftLeg, torso->position + Vec2(-20, +50));
//     // JointConstraint* rightHip = new JointConstraint(torso, rightLeg, torso->position + Vec2(+20, +50));
//     // world->AddConstraint(string);
//     // world->AddConstraint(neck);
//     // world->AddConstraint(leftShoulder);
//     // world->AddConstraint(rightShoulder);
//     // world->AddConstraint(leftHip);
//     // world->AddConstraint(rightHip);

//     Body* ball1 = new Body(CircleShape(200), (Graphics::Width() / 2.0) * 1.0, (Graphics::Height() / 2.0) * 1.0, 0.0);
//     ball1->restitution = 0.7;
//     world->AddBody(ball1);

//     Body* box1 = new Body(BoxShape(50, 1000), (Graphics::Width() / 3.0) * 1.0, (Graphics::Height() / 4.0) * 2.0, 0.0);
//     box1->restitution = 0.7;
//     world->AddBody(box1);
    
//     Body* box2 = new Body(BoxShape(50, 1000), (Graphics::Width() / 3.0) * 2.0, (Graphics::Height() / 4.0) * 2.0, 0.0);
//     box2->restitution = 0.7;
//     world->AddBody(box2);

//     Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
//     Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0 - 25, 0.0);
//     Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0 - 25, 0.0);
//     floor->restitution = 0.7;
//     leftWall->restitution = 0.2;
//     rightWall->restitution = 0.2;
//     world->AddBody(floor);
//     world->AddBody(leftWall);
//     world->AddBody(rightWall);

//     // Body* a = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
//     // Body* b = new Body(BoxShape(150, 150), 300, 0, 0.0);
//     // a->rotation = 0.0;
//     // b->rotation = 0.1;
//     // world->AddBody(a);
//     // world->AddBody(b);
// }

void Application::Setup() {
    running = Graphics::OpenWindow();

    // Create a physics world with gravity of -9.8 m/s2
    world = new World(-9.8);

    // Add bird
    Body* bird = new Body(CircleShape(45), 100, Graphics::Height() / 2.0 + 220, 3.0);
    world->AddBody(bird);

    // Add a floor and walls to contain objects objects
    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() / 2.0 + 290, 0.0);
    Body* leftFence = new Body(BoxShape(50, Graphics::Height() - 200), 0, Graphics::Height() / 2.0 - 35, 0.0);
    Body* rightFence = new Body(BoxShape(50, Graphics::Height() - 200), Graphics::Width(), Graphics::Height() / 2.0 - 35, 0.0);
    world->AddBody(floor);
    world->AddBody(leftFence);
    world->AddBody(rightFence);

    // Add a stack of boxes
    for (int i = 1; i <= 4; i++) {
        float mass = 10.0 / (float) i;
        Body* box = new Body(BoxShape(50, 50), 600, floor->position.y - i * 55, mass);
        box->friction = 0.9;
        box->restitution = 0.1;
        world->AddBody(box);
    }

    // Add structure with blocks
    Body* plank1 = new Body(BoxShape(50, 150), Graphics::Width() / 2.0 + 20, floor->position.y - 100, 5.0);
    Body* plank2 = new Body(BoxShape(50, 150), Graphics::Width() / 2.0 + 180, floor->position.y - 100, 5.0);
    Body* plank3 = new Body(BoxShape(250, 25), Graphics::Width() / 2.0 + 100.0f, floor->position.y - 200, 2.0);
    world->AddBody(plank1);
    world->AddBody(plank2);
    world->AddBody(plank3);

    // Add a triangle polygon
    std::vector<Vec2> triangleVertices = {Vec2(30, 30), Vec2(-30, 30), Vec2(0, -30)};
    Body* triangle = new Body(PolygonShape(triangleVertices), plank3->position.x, plank3->position.y - 50, 0.5);
    world->AddBody(triangle);

    // Add a pyramid of boxes
    int numRows = 5;
    for (int col = 0; col < numRows; col++) {
        for (int row = 0; row < col; row++) {
            float x = (plank3->position.x + 200.0f) + col * 50.0f - (row * 25.0f);
            float y = (floor->position.y - 50.0f) - row * 52.0f;
            float mass = (5.0f / (row + 1.0f));
            Body* box = new Body(BoxShape(50, 50), x, y, mass);
            box->friction = 0.9;
            box->restitution = 0.0;
            world->AddBody(box);
        }
    }

    // Add a bridge of connected steps and joints
    int numSteps = 10;
    int spacing = 33;
    Body* startStep = new Body(BoxShape(80, 20), 200, 200, 0.0);
    world->AddBody(startStep);
    Body* last = floor;
    for (int i = 1; i <= numSteps; i++) {
        float x = startStep->position.x + 30 + (i * spacing);
        float y = startStep->position.y + 20;
        float mass = (i == numSteps) ? 0.0 : 3.0;
        Body* step = new Body(CircleShape(15), x, y, mass);
        world->AddBody(step);
        JointConstraint* joint = new JointConstraint(last, step, step->position);
        world->AddConstraint(joint);
        last = step;
    }
    Body* endStep = new Body(BoxShape(80, 20), last->position.x + 60, last->position.y - 20, 0.0);
    world->AddBody(endStep);

    // Add pigs
    Body* pig1 = new Body(CircleShape(30), plank1->position.x + 80, floor->position.y - 50, 3.0);
    Body* pig2 = new Body(CircleShape(30), plank2->position.x + 400, floor->position.y - 50, 3.0);
    Body* pig3 = new Body(CircleShape(30), plank2->position.x + 460, floor->position.y - 50, 3.0);
    Body* pig4 = new Body(CircleShape(30), 220, 130, 1.0);
    world->AddBody(pig1);
    world->AddBody(pig2);
    world->AddBody(pig3);
    world->AddBody(pig4);
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
                    body = new Body(CircleShape(50), x, y, 1.0);
                    // body->SetTexture("./assets/basketball.png");
                    body->restitution = 0.9;
                    body->friction = 0.2;
                    world->AddBody(body);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* body;
                    body = new Body(BoxShape(100, 100), x, y, 1.0);
                    // body->SetTexture("./assets/crate.png");
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
            // case SDL_MOUSEMOTION:
            //     int x, y;
            //     SDL_GetMouseState(&x, &y);
            //     Body* box = world->GetBodies()[4];
            //     box->position.x = x;
            //     box->position.y = y;
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
    if (timeToWait > 0) 
        SDL_Delay(timeToWait);
    
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016;
    
    timePreviousFrame = SDL_GetTicks();

    world->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {

    // Body* bob = world->GetBodies()[0];
    // Body* head = world->GetBodies()[1];
    // Graphics::DrawLine(bob->position.x, bob->position.y, head->position.x, head->position.y, 0xFF555555);

    // Draw all joints anchor points
    // for (auto joint: world->GetConstraints()) {
    //     if (debug) {
    //         const Vec2 anchorPoint = joint->a->LocalSpaceToWorldSpace(joint->aPoint);
    //         Graphics::DrawFillCircle(anchorPoint.x, anchorPoint.y, 3, 0xFF0000FF);
    //     }
    // }

    for (auto body: world->GetBodies()) {
        Uint32 color = 0xFFFFFFFF;
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
