#pragma once

#include <HamFramework.hpp>
#include <Siv3D.hpp>

namespace Scenes {
    struct GameData {
    
    };

    using MyGame = SceneManager<String, GameData>;
}
#include "Scenes/Play.hpp"
#include "Scenes/Title.hpp"
#include "Scenes/Credit.hpp"
