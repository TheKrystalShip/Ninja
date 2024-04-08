#pragma once

// Toaster
#include "GLTexture.h"
#include "SpriteBatch.h"
#include "Vertex.h"
#include "GLTexture.h"

// External
#include "box2d/box2d.h"
#include <GLM/glm.hpp>

namespace Ninja
{
    class Box
    {
    public:
        Box();
        virtual ~Box();

        void init(b2World *world, const glm::vec2 position, const glm::vec2 &dimensions, Toaster::GLTexture texture, Toaster::ColorRGBA8 color, bool fixedRotation = false, glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        void draw(Toaster::SpriteBatch &spriteBatch);

        b2Body *getBody() const { return _body; }
        b2Fixture *getFixture() const { return _fixture; }
        const glm::vec2 &getDimensions() const { return _dimensions; }
        const Toaster::ColorRGBA8 &getColor() const { return _color; }

    private:
        glm::vec4 _uvRect;
        b2Body *_body = nullptr;
        b2Fixture *_fixture = nullptr;
        glm::vec2 _dimensions;
        Toaster::ColorRGBA8 _color;
        Toaster::GLTexture _texture;
    };
}
