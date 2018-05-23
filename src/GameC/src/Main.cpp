
# include <Siv3D.hpp>
# include <HamFramework.hpp>

#include "MacBundle.h"
#include "Util.hpp"
#include "ScenesBase.hpp"

void Main()
{
    Window::Resize(800, 600);
    
    Scenes::MyGame sceneManager;
    
    sceneManager.add<Scenes::TitleScene>(U"Title");
    sceneManager.add<Scenes::PlayScene>(U"Play");
    sceneManager.add<Scenes::CreditScene>(U"Credit");
    
    while (System::Update())
    {
        Window::SetTitle(U"{} (FPS: {})"_fmt(Scenes::Play::GameTitle, Profiler::FPS()));
        if(!sceneManager.update()) {
            return;
        }
    }
}
