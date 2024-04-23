#include "App.h"

#include <Toaster/ScreenList.h>

namespace Ninja
{
    App::App()
    {
    }

    App::~App()
    {
    }

    void App::onInit()
    {
    }

    void App::onExit()
    {
    }

    void App::addScreens()
    {
        _gameplayScreen = std::make_unique<GameplayScreen>(&_window);
        _screenList->addScreen(_gameplayScreen.get());
        _screenList->setScreen(_gameplayScreen->getIndex());
    }
}
