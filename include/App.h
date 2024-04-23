#pragma once

#include "GameplayScreen.h"

#include <Toaster/IMainGame.h>

#include <memory>

namespace Ninja
{
    class App : public Toaster::IMainGame
    {
    public:
        App();
        ~App();

        void onInit() override;
        void onExit() override;

        void addScreens() override;

    private:
        std::unique_ptr<GameplayScreen> _gameplayScreen = nullptr;
    };
}
