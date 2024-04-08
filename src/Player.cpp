#include "../include/Player.h"

// Toaster
#include <DebugRenderer.h>
#include <ResourceManager.h>

#include <GLM/glm.hpp>
#include <SDL2/SDL.h>

namespace Ninja
{
    Player::Player()
    {
    }

    Player::~Player()
    {
    }

    void Player::init(b2World *world, const glm::vec2 position, const glm::vec2 &drawDims, const glm::vec2 &collisionDims, Toaster::ColorRGBA8 color)
    {
        Toaster::GLTexture texture = Toaster::ResourceManager::getTexture("assets/textures/blue_ninja.png");
        _color = color;
        _drawDims = drawDims;

        _capsule.init(world, position, collisionDims, DENSITY, FRICTION, FIXED_ROTATION);
        _texture.init(texture, glm::ivec2(10, 2));
    }

    void Player::draw(Toaster::SpriteBatch &spriteBatch)
    {
        glm::vec4 destRect;
        auto body = _capsule.getBody();
        auto collisionDims = _capsule.getDimensions();

        destRect.x = body->GetPosition().x - _drawDims.x / 2.0f;
        destRect.y = body->GetPosition().y - collisionDims.y / 2.0f;
        destRect.z = _drawDims.x;
        destRect.w = _drawDims.y;

        // Calculate animation
        int numTiles = 1;
        int tileIndex = 0;
        glm::vec2 velocity(body->GetLinearVelocity().x, body->GetLinearVelocity().y);

        float animationSpeed = 0.2f;

        if (_isOnGround) // Player is touching the ground
        {
            if (_isPunching)
            {
                numTiles = 4;
                tileIndex = 1;
                if (_playerMoveState != PLAYER_MOVE_STATE::PUNCHING)
                {
                    _playerMoveState = PLAYER_MOVE_STATE::PUNCHING;
                    _animationTime = 0.0f;
                }
            }
            else if (std::abs(velocity.x) > 1.0f && ((velocity.x > 0.0f && _isFacingRight) || (velocity.x < 0.0f && !_isFacingRight)))
            {
                // Running
                numTiles = 6;
                tileIndex = 10;

                // Slow down animation based on velocity
                animationSpeed = std::abs(velocity.x) * 0.025f;

                if (_playerMoveState != PLAYER_MOVE_STATE::RUNNING)
                {
                    _playerMoveState = PLAYER_MOVE_STATE::RUNNING;
                    _animationTime = 0.0f;
                }
            }
            else
            {
                // Standing
                numTiles = 1;
                tileIndex = 0;
                _playerMoveState = PLAYER_MOVE_STATE::STANDING;
            }
        }
        else // Player is in the air
        {
            if (_isPunching)
            {
                numTiles = 1;
                tileIndex = 18;
                animationSpeed *= 0.5f;
                if (_playerMoveState != PLAYER_MOVE_STATE::PUNCHING)
                {
                    _playerMoveState = PLAYER_MOVE_STATE::PUNCHING;
                    _animationTime = 0.0f;
                }
            }
            else if (std::abs(velocity.x) > 10.0f)
            {
                numTiles = 1;
                tileIndex = 10;
            }
            else if (velocity.y <= 0.0f)
            {
                // Going down
                numTiles = 1;
                tileIndex = 17;
                _playerMoveState = PLAYER_MOVE_STATE::IN_AIR;
            }
            else
            {
                // Going up
                numTiles = 1;
                tileIndex = 16;
                _playerMoveState = PLAYER_MOVE_STATE::IN_AIR;
            }
        }

        // Increase animation time
        // TODO: this doesn't use deltaTime
        _animationTime += animationSpeed;

        // Cancel punching animation
        if (_animationTime > numTiles)
        {
            _isPunching = false;
        }

        // Apply animation over time
        tileIndex = tileIndex + (int)_animationTime % numTiles;

        auto uvRect = _texture.getUVs(tileIndex);

        // Check direction left/right
        if (!_isFacingRight)
        {
            // Move selection to next tile over
            uvRect.x += 1.0f / _texture.dims.x;
            // Mirror backwards to select previous tile but mirrored
            uvRect.z *= -1;
        }

        spriteBatch.draw(destRect, uvRect, _texture.texture.id, 0.0f, _color, body->GetAngle());
    }

    void Player::drawDebug(Toaster::DebugRenderer &debugRenderer)
    {
        _capsule.drawDebug(debugRenderer);
    }

    void Player::update(Toaster::InputManager &inputManager)
    {
        b2Body *body = _capsule.getBody();

        if (inputManager.isKeyDown(SDLK_a))
        {
            // Left
            body->ApplyForceToCenter(b2Vec2(-100.0f, 0.0f), true);
            _isFacingRight = false;
        }
        else if (inputManager.isKeyDown(SDLK_d))
        {
            // Right
            body->ApplyForceToCenter(b2Vec2(100.0f, 0.0f), true);
            _isFacingRight = true;
        }
        else if (inputManager.isKeyPressed(SDLK_SPACE))
        {
            _isPunching = true;
        }
        else
        {
            // Apply damping
            body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x * 0.9f, body->GetLinearVelocity().y));
        }

        // Max speed limiting
        if (body->GetLinearVelocity().x < -MAX_SPEED)
        {
            body->SetLinearVelocity(b2Vec2(-MAX_SPEED, body->GetLinearVelocity().y));
        }
        else if (body->GetLinearVelocity().x > MAX_SPEED)
        {
            body->SetLinearVelocity(b2Vec2(MAX_SPEED, body->GetLinearVelocity().y));
        }

        _isOnGround = false;

        // Loop through all contact point to check collision with ground
        for (b2ContactEdge *ce = body->GetContactList(); ce != nullptr; ce = ce->next)
        {
            b2Contact *contact = ce->contact;

            if (contact->IsTouching())
            {
                b2WorldManifold manifold;
                contact->GetWorldManifold(&manifold);

                // Check if points are below
                bool below = false;

                for (int i = 0; i < b2_maxManifoldPoints; i++)
                {
                    if (manifold.points[i].y < body->GetPosition().y - (_capsule.getDimensions().y / 2.0f) + (_capsule.getDimensions().x / 2.0f) + 0.01f)
                    {
                        below = true;
                        break;
                    }
                }

                if (below)
                {
                    _isOnGround = true;
                    // We can jump
                    if (inputManager.isKeyPressed(SDLK_w))
                    {
                        body->ApplyLinearImpulse(b2Vec2(0.0f, JUMP_HEIGHT), b2Vec2(0.0f, 0.0f), true);
                        break;
                    }
                }
            }
        }
    }
}
