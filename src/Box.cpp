#include "Box.h"

namespace Ninja
{
    Box::Box()
    {
    }

    Box::~Box()
    {
    }

    void Box::init(b2World *world, const glm::vec2 position, const glm::vec2 &dimensions, Toaster::GLTexture texture, Toaster::ColorRGBA8 color, bool fixedRotation /* = false */, glm::vec4 uvRect /* = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/)
    {
        _dimensions = dimensions;
        _color = color;
        _texture = texture;
        _uvRect = uvRect;

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.fixedRotation = fixedRotation;
        bodyDef.position.Set(position.x, position.y);

        _body = world->CreateBody(&bodyDef);

        b2PolygonShape boxShape;
        boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        _fixture = _body->CreateFixture(&fixtureDef);
    }

    void Box::draw(Toaster::SpriteBatch &spriteBatch)
    {
        glm::vec4 destRect;
        destRect.x = _body->GetPosition().x - _dimensions.x / 2.0f;
        destRect.y = _body->GetPosition().y - _dimensions.y / 2.0f;
        destRect.z = _dimensions.x;
        destRect.w = _dimensions.y;

        spriteBatch.draw(destRect, _uvRect, _texture.id, 0.0f, _color, _body->GetAngle());
    }
}
