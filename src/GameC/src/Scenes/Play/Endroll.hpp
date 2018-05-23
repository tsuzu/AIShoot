

namespace Scenes::Play {
    const Array<std::pair<String, Array<String>>> Credits {
        {U"---Program---", {U"Tsuzu", U"sugar", U"村岡正樹", U"西尾 正"}},
        {U"-----CG-----", {U"玄戸", U"げんげん"}},
        {U"----Music----", {U"むさしん"}},
        {U"---Library---", {U"OpenSiv3D(0.2.5)"}},
        {U"---Font---", {U"PixelMplus10", U"モトヤフォント"}},
    };
    
    class EndrollScene: public MyGame::Scene {
    private:
        Font titleFont{80};
        Font typFont{70, Scenes::Play::fontPMPlusPath};
        Font creditSmallFont{28, Typeface::Regular}, creditBigFont{36, Typeface::Bold};
        Texture hero{heroFrontPath};
        Texture heroine{heroinePath};
        Texture villain{villainPath};
        
        Texture heartTexture;
        Texture flowerTexture;
        
        Stopwatch scrollTimer{ true };
        
        struct CreditTextData {
            DrawableText dt;
            Vec2 pos;
        };
        
        Array<CreditTextData> creditTexts;
        std::function<std::pair<double, bool>()> charIconEasing;
        double alpha = 0.;
        
    public:
        EndrollScene(const InitData& init)
        : IScene(init),
        heartTexture(Play::heartBarIconPath),
        flowerTexture(Play::flowerBarIconPath),
        charIconEasing([]{return std::make_pair(0.0, false);})
        {
            creditTexts.reserve(64);

            double y = Window::Height();

            {
                auto dt = titleFont(U"Credit");
                const auto r = dt.region();
                Vec2 pos((Window::Width() - dt.region().w) / 2, y);
                
                y += r.h;
                
                creditTexts.emplace_back(CreditTextData{
                    std::move(dt),
                    std::move(pos),
                });
                
                y += 50;
            }

            for (const auto& credit : Credits) {
                auto dt = creditBigFont(credit.first);
                const auto r = dt.region();
                Vec2 pos((Window::Width() - dt.region().w) / 2, y);
                
                y += r.h;
                
                creditTexts.emplace_back(CreditTextData{
                    std::move(dt),
                    std::move(pos),
                });
                
                y += 10.;
                
                for (const auto& name : credit.second) {
                    auto dt = creditSmallFont(name);
                    const auto r = dt.region();
                    Vec2 pos((Window::Width() - dt.region().w) / 2, y);
                    
                    y += r.h;
                    
                    creditTexts.emplace_back(CreditTextData{
                        std::move(dt),
                        std::move(pos),
                    });
                    
                    y += 20.;
                }
                y += 30.;
            }
            
            y += Window::Height() - 60;

            {
                DrawableText typ = typFont(U"Thank you for playing!");
                const auto r = typ.region();
                
                creditTexts.emplace_back(CreditTextData{std::move(typ), Vec2{(Window::Width() - r.w) / 2, y}});
            
                y += r.h;
            }
            
        }
        
        bool easingStarted = false;
        void update() override {
            if (MouseL.down())
            {
                getData().isFinished = true;
                
                return;
            }

            double deltaY = System::DeltaTime() * 1000 / 20.;

            if(creditTexts.back().pos.y + creditTexts.back().dt.region().h > Window::Height() / 2) {
                for(auto& p : creditTexts) {
                    p.pos.y -= deltaY;
                }
            }else {
                if(!easingStarted){
                    easingStarted = true;
                    
                    charIconEasing = Util::EasingFunctionWithTime(EaseOut, Easing::Quad, 0., 1., Milliseconds(300));
                }
            }
            

            auto [alpha, _] = charIconEasing();
            this->alpha = alpha;
        }
        
        void draw() const override{
            for (int i = 0; i < 21; i++) {
                auto y = creditTexts.front().pos.y - 30. + i * 62;
                
                heartTexture.resized(32,32).drawAt(60, y);
                flowerTexture.resized(32,32).drawAt(Window::Width() - 60, y);
            }
            
            for (const auto& credit : creditTexts) {
                credit.dt.draw(credit.pos);
            }

            {
                hero.resized(120. / hero.height()  * hero.width(), 120.).drawAt(Window::Width() / 2 - 80, Window::Height() / 2. + 60, AlphaF(alpha));
                heroine.resized(120./ heroine.height()  * heroine.width(), 120.).drawAt(Window::Width() / 2, Window::Height() / 2. + 60, AlphaF(alpha));
                villain.resized(120. / villain.height() * villain.width(), 120.).drawAt(Window::Width() / 2 + 80, Window::Height() / 2. + 60, AlphaF(alpha));
            }
        }
    };
}
