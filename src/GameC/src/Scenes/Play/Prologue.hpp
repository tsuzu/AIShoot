


namespace Scenes::Play {
    class PrologueScene: public MyGame::Scene {
        Array<String> messages{
            U"愛子さんへの愛とプレゼントの花束が散らばってしまった！",
            U"告白を成功させるためにも頑張って集め直そう！",
        };
        
        Font charsTitleFont{56, fontMTLPath};
        
        Image image{Image(descriptionPath)};
        Texture blurred{image.gaussianBlurred(30, 30)};
        
        Texture hero{heroFrontPath,  TextureDesc::Mipped};
        Texture heroine{heroinePath, TextureDesc::Mipped};
        Texture villain{villainPath, TextureDesc::Mipped};
        
        Font charDescriptionFont{20};
        
        Array<Texture> textures;
        
        enum class Scene {
            Characters,
            Images,
            Text,
            Final,
        };
        Scene scene = Scene::Characters;
        int idx = 0;
        std::function<std::pair<double, bool>()> sceneChangerScreenAlpha = []{return std::make_pair(0., true);};
        double alpha = 0.;
        
        int easingStatus = 0;
        int lClickCount = 0;
        
        Font font{21};
        RoundRect frame{10, 500, 780, 90, 7};
        Rect dummyFrame{10, 495, 780, 90};
        int32 duration = 60;
        size_t messageIndex = 0;
        Stopwatch stopwatch{true};
        
        size_t previousIndex = 0;
        int32 index = 0;
        bool isAll = false, isFirst = true;
        
    public:
        
        PrologueScene(const InitData& init)
        : IScene(init) {
            for(int i = 1; i <= 5; ++i) {
                textures.push_back(Texture(resourceDir + U"/assets/prologue_{}.png"_fmt(i)));
            }
        }
        
        void update() override {
            switch(scene) {
                case Scene::Characters:
                case Scene::Images:{
                    bool updateEventOccurred = false;
                    
                    if(scene == Scene::Images && idx >= textures.size() - 1) {
                        scene = Scene::Text;
                        
                        return;
                    }
                    
                    if(MouseL.down()) {
                        if(easingStatus != 0)
                            updateEventOccurred = true;
                        
                        easingStatus = 1;
                        sceneChangerScreenAlpha = Util::EasingFunctionWithTime(EaseOut, Easing::Sine, 0., 1., Milliseconds(150));
                    }
                    
                    if(easingStatus == 1) {
                        auto [alpha, finished] = sceneChangerScreenAlpha();
                        
                        this->alpha = alpha;
                        
                        if(finished) {
                            sceneChangerScreenAlpha = Util::EasingFunctionWithTime(EaseOut, Easing::Sine, 1., 0., Milliseconds(150));
                            easingStatus = 2;
                            
                            updateEventOccurred = true;
                        }
                    }else if(easingStatus == 2) {
                        auto [alpha, finished] = sceneChangerScreenAlpha();
                        
                        this->alpha = alpha;
                        
                        if(finished) {
                            easingStatus = 0;
                        }
                    }
                    
                    switch (scene) {
                        case Scene::Characters:
                            if(updateEventOccurred) {
                                scene = Scene::Images;
                            }
                            
                            break;
                        case Scene::Images:
                            if(updateEventOccurred) {
                                ++idx;
                            }

                            break;
                    }
                    
                    break;
                }
                case Scene::Text:
                    if(isFirst) {
                        stopwatch.restart();
                        
                        this->alpha = 0.;
                        isFirst = false;
                    }
                    
                    previousIndex = messageIndex;
                    
                    index = stopwatch.ms() / duration;
                    
                    isAll = index >= messages[previousIndex].length();
                    
                    if (MouseL.down())
                    {
                        if (isAll)
                        {
                            ++messageIndex;
                            
                            if (messageIndex == messages.size()) {
                                changeScene(U"Description");
                                
                                return;
                            }
                            
                            messageIndex %= messages.size();
                            
                            stopwatch.restart();
                        }
                        else
                        {
                            stopwatch.set(1000000ms);
                        }
                    }
                    
                    break;
            }
        }
        
        void draw() const override {
            switch (scene) {
                case Scene::Characters:{
                    blurred.draw();
                    Rect(Window::Size()).draw(ColorF(0., 0., 0., 0.4));
                    
                    double d = 76.;
                    
                    const double x = 112.;
                    
                    auto title = charsTitleFont(U"登場人物紹介");
                    d = title.drawAt(Window::Width() / 2., d + title.region().h / 2., Palette::Hotpink).bottomCenter().y;
                    
                    d += 20.;
                    
                    {
                        auto tr = Util::Fit(hero, Util::Unspecified, 100.);
                        auto region = tr.region(x, d);
                        
                        tr.draw(region.pos);
                        charDescriptionFont(HeroIntroduction).draw(region.rightCenter().x, d);
                        
                        d = region.bottomCenter().y;
                    }
                    
                    d += 40.;
                    
                    {
                        auto tr = Util::Fit(heroine, Util::Unspecified, 100.);
                        auto region = tr.region(x, d);
                        
                        tr.draw(region.pos);
                        charDescriptionFont(HeroineIntroduction).draw(region.rightCenter().x, d);
                        
                        d = region.bottomCenter().y;
                    }

                    d += 40.;

                    {
                        auto tr = Util::Fit(villain, Util::Unspecified, 100.);
                        auto region = tr.region(x, d);
                        
                        tr.draw(region.pos);
                        charDescriptionFont(VillainIntroduction).draw(region.rightCenter().x, d);
                        
                        d = region.bottomCenter().y;
                    }
                    
                    break;
                }
                case Scene::Images:{
                    if(idx < textures.size()) {
                        textures[idx].resized(Window::Size()).draw();
                    }
                    
                    
                    break;
                }
                case Scene::Text:{
                    textures.back().resized(Window::Size()).draw();
                    
                    frame.stretched(2).draw();
                    frame.stretched(2).drawFrame(0, 2, Palette::Black);
                    
                    frame.stretched(-2).drawFrame(0, 2, Palette::Black);
                    
                    font(messages[previousIndex].substr(0, index)).draw(dummyFrame.pos + Point(30, 20), Palette::Black);
                    
                    if (isAll && stopwatch.ms() % 1000 < 500)
                    {
                        Triangle(dummyFrame.br() - Point(30, 40), 20, 180_deg).draw(Palette::Black);
                    }
                    
                    break;
                }
                case Scene::Final:{
                    
                    break;
                }
            }
            
            Rect(Point(0, 0), Window::Size()).draw(ColorF(0., 0., 0., alpha));
        }
    };
}
