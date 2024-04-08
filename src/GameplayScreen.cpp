// Local
#include "GameplayScreen.h"
#include "Box.h"

// Toaster
#include <IMainGame.h>
#include <ResourceManager.h>

// External
#include <GLM/glm.hpp>
#include <SDL2/SDL.h>

// STD
#include <random>

namespace Ninja
{
    GameplayScreen::GameplayScreen(Toaster::Window *window) : _window(window)
    {
    }

    GameplayScreen::~GameplayScreen()
    {
    }

    void GameplayScreen::build()
    {
    }

    void GameplayScreen::destroy()
    {
    }

    void GameplayScreen::onEntry()
    {
        b2Vec2 gravity(0.0f, -(GRAVITY * 2));
        _world = std::make_unique<b2World>(gravity);

        _debugRenderer.init();

        // Make the ground
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -25.0f);
        b2Body *groundBody = _world->CreateBody(&groundBodyDef);

        // Make ground fixture
        b2PolygonShape groundBox;
        // These are half size, so 50.0f becomes 100
        groundBox.SetAsBox(50.0f, 10.0f);
        groundBody->CreateFixture(&groundBox, 0.0f);

        // Load texture
        _texture = Toaster::ResourceManager::getTexture("assets/textures/bricks_top.png");

        std::mt19937 randGen;
        std::uniform_real_distribution<float> xDist(-10.0f, 10.0f);
        std::uniform_real_distribution<float> yDist(-10.0f, 15.0f);
        std::uniform_real_distribution<float> sizeDist(0.5f, 2.5f);
        std::uniform_int_distribution<int> colorDist(0, 255);

        const int NUM_BOXES = 50;

        for (int i = 0; i < NUM_BOXES; i++)
        {
            Toaster::ColorRGBA8 randColor(colorDist(randGen), colorDist(randGen), colorDist(randGen));
            Box newBox;
            newBox.init(_world.get(), glm::vec2(xDist(randGen), yDist(randGen)),
                        glm::vec2(sizeDist(randGen), sizeDist(randGen)), _texture, randColor);
            _boxes.push_back(newBox);
        }

        // Initialize spritebatch
        _spriteBatch.init();

        // Shaders
        _program.compileShaders("assets/shaders/textureShading.vert", "assets/shaders/textureShading.frag");
        _program.addAttribute("vertexPosition");
        _program.addAttribute("vertexColor");
        _program.addAttribute("vertexUV");
        _program.linkShaders();

        // Camera setup
        _camera.init(_window->getWidth(), _window->getHeight());
        _camera.setScale(32.0f);

        // Player setup
        _player.init(_world.get(), glm::vec2(0.0f, 30.0f), glm::vec2(2.0f), glm::vec2(1.0f, 1.8f), Toaster::ColorRGBA8(255, 255, 255));
    }

    void GameplayScreen::onExit()
    {
        _debugRenderer.dispose();
    }

    void GameplayScreen::update()
    {
        _camera.update();
        checkInput();
        _player.update(_game->inputManager);

        // Update physics simulation
        _world->Step(1.0 / 60.0f, 6, 2);
    }

    void GameplayScreen::draw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

        _program.bind();

        // Upload texture uniform
        GLint textureUniform = _program.getUniformLocation("mySampler");
        glUniform1i(textureUniform, 0);
        glActiveTexture(GL_TEXTURE0);

        // Camera matrix
        glm::mat4 projectionMatrix = _camera.getCameraMatrix();
        GLint pUniform = _program.getUniformLocation("P");
        glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

        _spriteBatch.begin();

        // Draw all boxes
        for (auto &b : _boxes)
        {
            b.draw(_spriteBatch);
        }

        // Render player
        _player.draw(_spriteBatch);

        _spriteBatch.end();
        _spriteBatch.renderBatch();
        _program.unbind();

        if (_debug)
        {
            glm::vec4 destRect;
            for (auto &b : _boxes)
            {
                // Use DebugRenderer to show white outlines of collision boxes
                destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
                destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
                destRect.z = b.getDimensions().x;
                destRect.w = b.getDimensions().y;

                _debugRenderer.drawBox(destRect, Toaster::ColorRGBA8(255, 255, 255), b.getBody()->GetAngle());
            }

            _player.drawDebug(_debugRenderer);

            _debugRenderer.end();
            _debugRenderer.render(projectionMatrix, 2.0f);
        }
    }

    int GameplayScreen::getNextIndex() const
    {
        return INVALID_SCREEN_INDEX;
    }

    int GameplayScreen::getPreviousIndex() const
    {
        return INVALID_SCREEN_INDEX;
    }

    void GameplayScreen::checkInput()
    {
        SDL_Event evnt;
        while (SDL_PollEvent(&evnt))
            _game->onSDLEvent(evnt);
    }
}
