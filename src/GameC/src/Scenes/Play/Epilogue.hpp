


namespace Scenes::Play {
    class EpilogueScene: public MyGame::Scene {
        Texture texture;
        Array<String> messages;
        
        Font font{21};
        RoundRect frame{10, 500, 780, 90, 7};
        Rect dummyFrame{10, 495, 780, 90};
        int32 duration = 60;
        size_t messageIndex = 0;
        Stopwatch stopwatch{true};
        
        bool showTexts = true, textFinished = false;
    public:
        EpilogueScene(const InitData& init)
        : IScene(init) {
            auto& e = Ends.at(getData().endKind);
            
            texture = Texture(e.first);
            messages = e.second;
        }
        
        size_t previousIndex = 0;
        int32 index = 0;
        bool isAll = false, isFirst = true;
        
        void update() override {
            if(isFirst) {
                stopwatch.restart();
                
                isFirst = false;
            }
            
            if(textFinished) {
                if(MouseL.down()) {
                    changeScene(U"Endroll");
                }
                return;
            }
            previousIndex = messageIndex;
            
            index = stopwatch.ms() / duration;
            
            isAll = index >= messages[previousIndex].length();
            
            if (MouseL.down() && !MouseR.pressed())
            {
                if (isAll)
                {
                    ++messageIndex;
                    
                    if (messageIndex == messages.size()) {
                        textFinished = true;
                    }
                    
                    messageIndex %= messages.size();
                    
                    stopwatch.restart();
                }
                else
                {
                    stopwatch.set(1000000ms);
                }
            }
            
            if(MouseR.down()) {
                showTexts = !showTexts;
            }
        }
        
        void draw() const override {
            texture.resized(Window::Size()).draw();
            
            if(showTexts && !textFinished) {
                frame.stretched(2).draw();
                frame.stretched(2).drawFrame(0, 2, Palette::Black);
                
                frame.stretched(-2).drawFrame(0, 2, Palette::Black);
                
                font(messages[previousIndex].substr(0, index)).draw(dummyFrame.pos + Point(30, 20), Palette::Black);
                
                if (isAll && stopwatch.ms() % 1000 < 500)
                {
                    Triangle(dummyFrame.br() - Point(30, 40), 20, 180_deg).draw(Palette::Black);
                }
            }
        }
    };
}
