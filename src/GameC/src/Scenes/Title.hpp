

namespace Scenes {
    class TitleScene: public MyGame::Scene {
        Font titleFont{80, Scenes::Play::fontMTLPath};
        Font menuFont{26, Typeface::Bold};
        
        Array<RoundRect> title_menubox;
        Array<String> menu_opt = {U"Game Start", U"Credits"};
        Texture background;
        Texture flowerTexture;
        Texture heartTexture;
        Texture heroTexture;
        
    public:
        TitleScene(const InitData& init)
        : IScene(init),
        flowerTexture(Play::flowerBarIconPath,  TextureDesc::Mipped),
        heartTexture(Play::heartBarIconPath,  TextureDesc::Mipped),
        heroTexture(Play::heroPath,  TextureDesc::Mipped),
        background(Image(Play::backgroundTitlePath).gaussianBlurred(30., 30.))
        {
            title_menubox.resize(menu_opt.size());
            for (auto i : step(title_menubox.size())) {
                title_menubox[i].set(250, 400 + static_cast<int>(i) * 80, 300, 60, 7);
            }
        }
        
        void update() override {
            for (auto i : step(title_menubox.size())) {
                const auto& item = title_menubox[i];
                
                if (item.leftClicked()) {
                    if (i == 0) {
                        changeScene(U"Play");
                    } else if (i == 1) {
                        changeScene(U"Credit");
                    }
                    break;
                }
            }
        }
        
        void draw() const override {
            background.resized(Window::Size()).draw();
            
            titleFont(Play::GameTitle).drawAt(Window::Center().x, 180);
            for (auto i : step(title_menubox.size())) {
                if(title_menubox[i].mouseOver() && !MouseL.pressed()) {
                    title_menubox[i].drawShadow(Vec2(6.,6.), 1.);
                }

                title_menubox[i].draw();
                
                menuFont(menu_opt[i]).drawAt(title_menubox[i].center(), Palette::Hotpink);
            }
            
            flowerTexture.resized(96, 96).drawAt(140, 120);
            flowerTexture.resized(96, 96).drawAt(660, 380);
            heartTexture.resized(96,96).drawAt(640,110);
            heartTexture.resized(96,96).drawAt(120,420);
            heroTexture.resized(112,112).drawAt(Window::Center().x, Window::Center().y);
        }
    };
}
