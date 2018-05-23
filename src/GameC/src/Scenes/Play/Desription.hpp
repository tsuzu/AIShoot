


namespace Scenes::Play {
    class DescriptionScene: public MyGame::Scene {
        Polygon rightArrow = Polygon{
            {0, 0}, {-20, -20}, {-20, 20},
        };
        
        Array<String> messages{GameDescription};
        
        Array<std::function<void(bool/*isFirst*/, TextureRegion/*texture*/, Vec2/*pos*/)>> additionalDrawing{GameDescriptionDrawing};

        Image image{Image(descriptionPath)};
        Texture blurred{image.gaussianBlurred(30, 30)};
        TextureRegion texture  = Util::Fit(Texture{image}, Util::Unspecified, 500.);
        
        Font font{21};
        RoundRect frame{10, 500, 780, 90, 7};
        Rect dummyFrame{10, 495, 780, 90};
        int32 duration = 60;
        size_t messageIndex = 0;
        Stopwatch stopwatch{true};
        
    public:
        DescriptionScene(const InitData& init)
        : IScene(init) {

        }
        
        size_t previousIndex = 0;
        int32 index = 0;
        bool isAll = false, isFirst = true;
        bool isIndexChanged = false;
        
        void update() override {
            if(isFirst) {
                stopwatch.restart();
                
                isFirst = false;
            }
            
            if(rightArrow.movedBy(Window::Width() - 16., 36.).leftClicked()) {
                changeScene(U"Game");
                
                return;
            }
            
            previousIndex = messageIndex;
            
            index = stopwatch.ms() / duration;
            
            isAll = index >= messages[previousIndex].length();
            
            if (MouseL.down() && !MouseR.pressed())
            {
                if (isAll)
                {
                    isIndexChanged = true;

                    ++messageIndex;
                    
                    if (messageIndex == messages.size()) {
                        changeScene(U"Game");
                    }
                    
                    messageIndex %= messages.size();
                    
                    stopwatch.restart();
                }
                else
                {
                    stopwatch.set(1000000ms);
                }
            }else {
                isIndexChanged = false;
            }
            
        }
        
        void draw() const override {
            blurred.draw();
            
            const Vec2 texturePos(Window::Width() / 2, texture.size.y / 2);
            
            texture.drawAt(texturePos);
            
            additionalDrawing[previousIndex](isIndexChanged, texture, texturePos);
            
            if(rightArrow.movedBy(Window::Width() - 16., 36.).mouseOver() && !MouseL.pressed()) {
                rightArrow.movedBy(5., 5.).draw(Window::Width() - 16., 36., ColorF(0, 0, 0, 0.4));
            }

            rightArrow.draw(Window::Width() - 16., 36.);
            
            frame.stretched(2).draw();
            frame.stretched(2).drawFrame(0, 2, Palette::Black);
            
            frame.stretched(-2).drawFrame(0, 2, Palette::Black);
            
            font(messages[previousIndex].substr(0, index)).draw(dummyFrame.pos + Point(30, 20), Palette::Black);
            
            if (isAll && stopwatch.ms() % 1000 < 500)
            {
                Triangle(dummyFrame.br() - Point(30, 40), 20, 180_deg).draw(Palette::Black);
            }
        }
    };
}
