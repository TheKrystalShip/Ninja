#pragma once

// Toaster
#include <Toaster/DebugRenderer.h>

// Box2d
#include <box2d/box2d.h>

// GLM
#include <GLM/glm.hpp>

namespace Ninja
{
    class Capsule
    {
    public:
        Capsule();
        ~Capsule();

        void init(b2World *world, const glm::vec2 &position, const glm::vec2 &dimensions, float density, float friction, bool fixedRotation = false);
        void drawDebug(Toaster::DebugRenderer &debugRenderer);

        b2Body *getBody() const { return _body; }
        b2Fixture *getFixture(int index) const { return _fixtures[index]; }
        const glm::vec2 &getDimensions() const { return _dimensions; }

    private:
        b2Body *_body = nullptr;
        b2Fixture *_fixtures[3];
        glm::vec2 _dimensions;
    };
}
