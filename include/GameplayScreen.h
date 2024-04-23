#pragma once

// Local
#include "Box.h"
#include "Player.h"

// Toaster
#include <Toaster/Camera2D.h>
#include <Toaster/DebugRenderer.h>
#include <Toaster/GLSLProgram.h>
#include <Toaster/Texture2D.h>
#include <Toaster/IGameScreen.h>
#include <Toaster/SpriteBatch.h>
#include <Toaster/Window.h>

// Box2D
#include <box2d/box2d.h>

// STD
#include <memory>
#include <vector>

namespace Ninja
{
    class GameplayScreen : public Toaster::IGameScreen
    {
    public:
        static constexpr float GRAVITY = 9.81f;

        GameplayScreen(Toaster::Window *window);
        virtual ~GameplayScreen();

        void build() override;
        void destroy() override;

        // Called when screen enters/leaves focus
        void onEntry() override;
        void onExit() override;

        void update() override;
        void draw() override;

        int getNextIndex() const override;
        int getPreviousIndex() const override;

    private:
        void checkInput();

        Toaster::SpriteBatch _spriteBatch;
        Toaster::GLSLProgram _program;
        Toaster::Camera2D _camera;
        Toaster::Texture2D _texture;
        Toaster::Window *_window = nullptr;
        Toaster::DebugRenderer _debugRenderer;

        bool _debug = false;

        Player _player;

        std::vector<Box> _boxes;
        std::unique_ptr<b2World> _world;
    };
}
