


namespace Scenes::Play {
    class GameScene: public MyGame::Scene {
        enum class ObjectType {
            Obstacle,
            Heart,
            Flower,
        };
        
        const double StatusBarHeight = Window::Height() * 3 / 12;
        const double GameStageHeight = Window::Height() * 8 / 12;
        const double PlayerBarHeight = Window::Height() * 1 / 12;
        
        const double BarIconSize = 60;
        const double ObjectSize = 60;
        
        const double HeroWidth = 90 / 275. * 490.;
        const double HeroHeight = 90;
        const double HeroineWidth = StatusBarHeight / 430. * 300.;
        const double HeroineHeight = StatusBarHeight;
        const double VillainHeight = 90.;
        const double VillainWidth = 90. / 440. * 340.;
        
        const double InformationBarWidth = (Window::Width() - HeroineWidth) / 2 - BarIconSize;
        const double InformationBarHeight = 40;
        
        const Vec2 PortPos{Window::Width() / 2, /*StatusBarHeight + GameStageHeight*/590};
        const Rect gameStageRect = Rect(0, StatusBarHeight, Window::Width(), GameStageHeight + PlayerBarHeight);
        
        class Bullet {
            Vec2 velocity;
            Rect stage;
            bool isActive_ = true;
            
        public:
            Quad quad;
            
            Bullet() = default;
            Bullet(Bullet&&) = default;
            Bullet& operator = (Bullet&&) = default;
            
            Bullet(Vec2 pos, double angle, Vec2 velocity, Rect stage):
            velocity(velocity), stage(stage)
            {
                quad = bulletRect.rotatedAt(0., 0., angle).movedBy(pos);
            }
            
            void update() {
                if (!isActive_) {
                    return;
                }
                
                quad.moveBy(velocity);
                
                if (!stage.intersects(quad)) {
                    isActive_ = false;
                }
            }
            
            void draw() const {
                quad.draw();
            }
            
            bool isActive() const {
                return isActive_;
            }
            
            void disable() {
                isActive_ = false;
            }
            
        };
        
        class FloatingObject {
        public:
            ObjectType type;
        private:
            Polygon poly;
            TextureRegion tex;
            Vec2 pos;
            double velocity;
            Rect stage; // stageと接触しなくなると消える
            
            bool isActive_ = true; // eraseしていいかどうか
            bool isDisappearing = false; // 消えていいかどうか
            Stopwatch watch;
            
            double alpha = 1.0;
        public:
            FloatingObject(ObjectType type, TextureRegion tex, Polygon poly, Vec2 pos, double velocity, Rect stage):
            tex(tex), poly(poly),type(type), pos(pos), velocity(velocity), stage(stage){
                this->poly.moveBy(pos);

            }
            
            void update() {
                if (!isActive_) {
                    return;
                }
                
                if(watch.isRunning()) {
                    double t = Min(watch.ms() / 200., 1.0);
                    
                    alpha = EaseOut(Easing::Quart, 1.0, 0.0, t);
                    
                    if (t == 1.0) {
                        isActive_ = false;
                        watch.reset();
                    }
                    
                    return;
                }
                
                pos.x += velocity;
                poly.moveBy(Vec2(velocity, 0.));
                
                if(Window::Width() <= pos.x) {
                    isActive_ = false;
                }
            }
            
            void draw() const {
                if(!isActive_) {
                    return;
                }
                
                this->tex.draw(pos, AlphaF(alpha));
            }
            
            bool isActive() const {
                return isActive_;
            }
            
            bool intersects(const Bullet& b) {
                if(!isActive_ || isDisappearing) {
                    return false;
                }
                
                
                if(poly.intersects(b.quad)) {
                    startDisappearing();
                    
                    return true;
                }
                
                return false;
            }
            
        private:
            void startDisappearing() {
                isDisappearing = true;
                watch.start();
            }
        };
        
        class InfoBar {
            Texture& icon;
            double x, y, w, h;
            Color barColor;
            Stopwatch watch;
            
        public:
            double source = 0., destination = 0.;
            double current = 0.;
            
            InfoBar() = default;
            InfoBar(InfoBar&) = default;
            InfoBar(InfoBar&&) = default;
            InfoBar& operator = (InfoBar&) = default;
            InfoBar& operator = (InfoBar&&) = default;
            
            InfoBar(Texture& icon, Color barColor, double x, double y, double w, double h):
            icon(icon), x(x), y(y), w(w), h(h), barColor(barColor)
            { // must be w > h
                
            }
            
            void setAnimation(double destination) {
                source = current;
                this->destination = destination;
                watch.restart();
            }
            
            void update()  {
                if (watch.isRunning()) {
                    double t = Min(watch.ms() / 300.0, 1.0);
                    
                    current = EaseOut(Easing::Quart, source, destination, t);
                    
                    if (t == 1.00) {
                        watch.reset();
                    }
                }
            }
            
            void draw() const  {
                icon.resized(h).draw(x + w - h, y);
                RoundRect(x, y + 10, w - h, h - 20, 7).draw(Color(230, 230, 230));
                RoundRect(x, y + 10, w - h, h - 20, 7).draw(Color(barColor.r, barColor.g, barColor.b, 30));
                RoundRect(x, y + 10, (w - h) * current, h - 20, 7).draw(barColor);
                Line(x + (w - h) * 0.8, y + 10, x + (w - h) * 0.8, y + h - 10).draw(Color(80, 80, 80));
            }
        };
        
        Audio goodSE{goodSEPath}, badSE{badSEPath}, mainBGM{mainBGMPath};
        
        Image heartImage{heartBarIconPath}, flowerImage{flowerBarIconPath}, obstacleImage{obstaclePath};
        Texture heartTexture{heartImage,  TextureDesc::Mipped};
        Texture flowerTexture{flowerImage,  TextureDesc::Mipped};
        Texture obstacleTexture{obstacleImage,  TextureDesc::Mipped};
        
        Texture heroCut{heroCutPath, TextureDesc::Mipped};
        Texture heroine{heroinePath,  TextureDesc::Mipped};
        Texture villain{villainPath,  TextureDesc::Mipped};
        Texture villainSmall{villainSmallPath, TextureDesc::Mipped};
        
        TextureRegion backgroundRoad{Texture(backgroundRoadPath, TextureDesc::Mipped).resized(Window::Width(), GameStageHeight)};
        TextureRegion backgroundUpper{Texture(backgroundUpperPath, TextureDesc::Mipped).resized(Window::Width(), StatusBarHeight)};
        TextureRegion grass{Texture(grassPath, TextureDesc::Mipped).scaled(0.5)};
        Polygon heartPoly, flowerPoly, obstaclePoly;
        
        Array<Bullet> bullets;
        Array<FloatingObject> objects;
        double heartAccumulation = 0.; // [0., 100.]
        double flowerAccumulation = 0.; // [0., 100.]
        
        InfoBar heartBar{heartTexture, Color(208,45,53), (Window::Width() + HeroineWidth) / 2, 10, Window::Width() - (Window::Width() + HeroineWidth) / 2, BarIconSize};
        InfoBar flowerBar{flowerTexture, Color(239,182,63), (Window::Width() + HeroineWidth) / 2, 10 * 2 + BarIconSize, Window::Width() - (Window::Width() + HeroineWidth) / 2, BarIconSize};
        Timer timer{DurationCast<Milliseconds>(Seconds(Timelimit))};
        
        int beforeCount[4] = {};
        
        void validateAccumulation(double& d) {
            if(d > 100.) {
                d = 100.0;
            }else if(d < 0.){
                d = 0.0;
            }
        }
        
    public:
        GameScene(const InitData& init)
        : IScene(init)
        {
            heartPoly = ImageProcessing::FindExternalContour(heartImage, true).scaled(ObjectSize / heartImage.width());
            flowerPoly = ImageProcessing::FindExternalContour(flowerImage, true).scaled(ObjectSize / flowerImage.width());
            obstaclePoly = ImageProcessing::FindExternalContour(obstacleImage, true).scaled(ObjectSize / obstacleImage.width());

            timer.start();
        }
        
        ~GameScene() {
            if(mainBGM.isPlaying()) {
                mainBGM.stop(SecondsF(2.0));
            }
        }
        
        bool isFirst = true;
        int32 elapsedInSeconds = 0;
        void update() override {
            if(isFirst) {
                mainBGM.setLoop(Arg::loopBegin=SecondsF(0), Arg::loopEnd=SecondsF(12));
                mainBGM.play(SecondsF(2.));
                timer.start();
                elapsedInSeconds = timer.s();
                
                isFirst = false;
            }
            
            if(timer.reachedZero()) {
                changeScene(U"Epilogue");
                
                
                auto& kind = getData().endKind;
                
                if(heartAccumulation >= 80.0 && flowerAccumulation >= 80.0) {
                    kind = EndKind::TrueEnd;
                }else if(heartAccumulation >= 80.0) {
                    kind = EndKind::IkemenEnd;
                }else if(flowerAccumulation >= 80.0){
                    kind = EndKind::BadEnd;
                }else {
                    kind = EndKind::GameOver;
                }
                
            }
            
            for(auto& b : bullets) {
                b.update();
            }
            
            for(auto& o : objects) {
                o.update();
            }
            
            bullets.remove_if([](const auto& t) {
                return !t.isActive();
            });
            
            objects.remove_if([](const auto& t) {
                return !t.isActive();
            });
            
            bool good = false, bad = false;
            for(auto& o: objects) {
                bool coll = false;
                for(auto& b: bullets) {
                    if(o.intersects(b)) {
                        b.disable();
                        coll = true;
                        
                        break;
                        
                    }
                }
                
                if(coll) {
                    switch(o.type) {
                        case ObjectType::Heart:
                            heartAccumulation += 10.0;
                            validateAccumulation(heartAccumulation);
                            
                            heartBar.setAnimation(heartAccumulation / 100.);
                            
                            good = true;
                            break;
                            
                        case ObjectType::Flower:
                            flowerAccumulation += 10.0;
                            validateAccumulation(flowerAccumulation);

                            flowerBar.setAnimation(flowerAccumulation / 100.);

                            good = true;
                            break;

                        case ObjectType::Obstacle:
                            heartAccumulation -= 15.0;
                            flowerAccumulation -= 15.0;
                            validateAccumulation(heartAccumulation);
                            validateAccumulation(flowerAccumulation);
                            heartBar.setAnimation(heartAccumulation / 100.);
                            flowerBar.setAnimation(flowerAccumulation / 100.);

                            bad = true;

                    }
                    
                }
            }
            
            if(good) {
                goodSE.playOneShot();
            }
            if (bad) {
                badSE.playOneShot();
            }
            
            if(elapsedInSeconds != timer.s()) {
                elapsedInSeconds = timer.s();
                
                heartAccumulation -= 0.75;
                flowerAccumulation -= 0.75;
                validateAccumulation(heartAccumulation);
                validateAccumulation(flowerAccumulation);
                heartBar.setAnimation(heartAccumulation / 100.);
                flowerBar.setAnimation(flowerAccumulation / 100.);
            }

            auto frame = System::FrameCount();
            for(int i = 0; i < 3; ++i) {
                if(!(frame - beforeCount[i] >= 20 && Random(0, 60) == 0)) {
                    continue;
                }

                beforeCount[i] = frame;
                
                double y = StatusBarHeight + 45 + i* 90;
                int typev = Random(0, 2);
                ObjectType type;
                Texture tex;
                Polygon poly;
                
                if (typev == 0) {
                    type = ObjectType::Obstacle;
                    tex = obstacleTexture;
                    poly = obstaclePoly;
                    
                }else if(typev == 1) {
                    type = ObjectType::Heart;
                    tex = heartTexture;
                    poly = heartPoly;
                }else {
                    type = ObjectType::Flower;
                    tex = flowerTexture;
                    poly = flowerPoly;
                }
                
                auto t = tex.resized(ObjectSize);
                
                objects.push_back(FloatingObject(type, t, poly, Vec2(-ObjectSize, y - ObjectSize / 2), 3., gameStageRect));
            }
            
            if(Rect(0, StatusBarHeight, Window::Width(), GameStageHeight).leftClicked()) {
                auto angleVec = Cursor::Pos() - PortPos;
                
                auto angle = atan2(angleVec.y, angleVec.x);
                auto unit = Util::unitVector(angleVec);
                
                bullets.push_back(Bullet(PortPos, angle, unit * 7, gameStageRect));
            }
            
            heartBar.update();
            flowerBar.update();
        }
        
        void draw() const override {
            
            Rect(0, StatusBarHeight + GameStageHeight, Window::Width(), PlayerBarHeight).draw(Palette::Cornsilk);
            
            backgroundRoad.draw(0, StatusBarHeight);
            
            for(auto& b : bullets) {
                b.draw();
            }
            
            heroCut.resized(HeroWidth, HeroHeight).draw((Window::Width() - HeroWidth) / 2, StatusBarHeight + GameStageHeight - HeroHeight + PlayerBarHeight);

            backgroundUpper.draw(0, 0);
            heroine.resized(HeroineWidth, HeroineHeight).draw((Window::Width() - HeroineWidth) / 2, 0);
            heartBar.draw();
            flowerBar.draw();

            villainSmall
            .resized(VillainWidth, VillainHeight)
            .draw(
                  (Window::Width() - HeroineWidth) / 2 * timer.progress0_1() - VillainWidth,
                  (StatusBarHeight - VillainHeight) / 2. - 5. + 9 * Sin(timer.progress0_1() * 100_pi)
                  );

            grass.draw();

            for(auto& o : objects) {
                o.draw();
            }
            
        }
    };
}
