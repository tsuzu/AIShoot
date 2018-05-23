namespace Scenes {
    enum class EndKind {
        TrueEnd,
        IkemenEnd,
        BadEnd,
        GameOver,
    };
    
    namespace Play {
        struct GameData: public ::Scenes::GameData {
            bool isFinished; // change to title
            
            EndKind endKind;
        };
        
        using MyGame = SceneManager<String, GameData>;
    }
}

#include "./Play/Consts.hpp"

#include "./Play/Prologue.hpp"
#include "./Play/Desription.hpp"
#include "./Play/Game.hpp"
#include "./Play/Epilogue.hpp"
#include "./Play/Endroll.hpp"


namespace Scenes {
    class PlayScene: public MyGame::Scene { // manager
        Play::MyGame manager;
        
    public:
        PlayScene(const InitData& init)
        : IScene(init) {
            manager.add<Play::PrologueScene>(U"Prologue");
            manager.add<Play::DescriptionScene>(U"Description");
            manager.add<Play::GameScene>(U"Game");
            manager.add<Play::EpilogueScene>(U"Epilogue");
            manager.add<Play::EndrollScene>(U"Endroll");
            
            manager.init(U"Prologue");
        }
        
        void update() override {
            manager.updateScene();
            
            if(manager.get()->isFinished) {
                changeScene(U"Title");
            }
        }
        
        void draw() const override {
            manager.drawScene();
        }
    };
}
