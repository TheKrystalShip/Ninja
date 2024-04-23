#pragma once

#include "Capsule.h"

// Toaster
#include <InputManager.h>
#include <SpriteBatch.h>
#include <TileSheet.h>

namespace Ninja
{
    enum class PLAYER_MOVE_STATE
    {
        STANDING,
        RUNNING,
        PUNCHING,
        IN_AIR
    };

    class Player
    {
    public:
        static constexpr float JUMP_HEIGHT = 50.0f;
        static constexpr float MAX_SPEED = 10.0f;
        static constexpr float DENSITY = 1.5f;
        static constexpr float FRICTION = 0.1f;
        static constexpr float FIXED_ROTATION = true;

        Player();
        virtual ~Player();

        void init(b2World *world, const glm::vec2 position, const glm::vec2 &drawDims, const glm::vec2 &collisionDims, Toaster::ColorRGBA8 colorRGBA8);

        void update(Toaster::InputManager &inputManager);

        void draw(Toaster::SpriteBatch &spriteBatch);
        void drawDebug(Toaster::DebugRenderer &debugRenderer);

        const Capsule &getCapsule() const { return _capsule; }

    private:
        glm::vec2 _drawDims;
        Capsule _capsule;
        Toaster::TileSheet _texture;
        Toaster::ColorRGBA8 _color;
        float _animationTime = 0.0f;
        bool _isFacingRight = true;
        bool _isPunching = false;
        bool _isOnGround = false;
        PLAYER_MOVE_STATE _playerMoveState = PLAYER_MOVE_STATE::STANDING;
    };
}
