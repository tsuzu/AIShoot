#include "CreditData.hpp"

namespace Scenes {
    using namespace std::placeholders;
    
    class CreditScene: public MyGame::Scene {
        
        enum class Mode {
            Authors,
            S3DLicense,
            MotoyaLicense,
        };
        
        Font titleFont = Font(40);
        Font licenseFont = Font(12);
        Font authorsFont = Font(16);
        Array<std::tuple<Mode, std::function<void(double&)>, std::function<void(double, double)>>> modes {
            {
                Mode::Authors,
                std::function<void(double&)>(std::bind(&CreditScene::updateAuthors, this, _1)),
                std::function<void(double, double)>(std::bind(&CreditScene::drawAuthors, this, _1, _2)),
            },
            {
                Mode::S3DLicense,
                std::function<void(double&)>(std::bind(&CreditScene::updateS3DLicense, this, _1)),
                std::function<void(double, double)>(std::bind(&CreditScene::drawS3DLicense, this, _1, _2)),
            },
            {
                Mode::MotoyaLicense,
                std::function<void(double&)>(std::bind(&CreditScene::updateMotoyaLicense, this, _1)),
                std::function<void(double, double)>(std::bind(&CreditScene::drawMotoyaLicense, this, _1, _2)),
            },
        };
        Array<double> scrolls = Array<double>(modes.size(), 0.);
        
        Polygon leftArrow = Polygon{
            {0, 0}, {20, 20}, {20, -20},
        };
        Polygon rightArrow = Polygon{
            {0, 0}, {-20, -20}, {-20, 20},
        };
        
        double currentOffset = 0.;
        int modeIdx = 0;
        Stopwatch watch;
        std::function<double(double)> easing;
        
    public:
        CreditScene(const InitData& init)
        : IScene(init)
        {
        }
        
        
        void update() override {
            if(leftArrow.movedBy(16., 36.).leftClicked() || KeyBackspace.down()) {
                changeScene(U"Title");
                
                return;
            }
            
            
            if(
               (KeyLeft.down() || leftArrow.movedBy(16., Window::Height() / 2).leftClicked())
               && modeIdx != 0
               ) {
                --modeIdx;
                scrolls[modeIdx] = 0.;
                
                watch.restart();
                
                easing = Util::EasingFunction(EaseOut, Easing::Quad, currentOffset, static_cast<double>(modeIdx) * Window::Width());

            }
            
            if(
               (KeyRight.down() || rightArrow.movedBy(Window::Width() - 16., Window::Height() / 2).leftClicked())
               && modeIdx != modes.size() - 1
               ) {
                ++modeIdx;
                scrolls[modeIdx] = 0.;

                watch.restart();
                
                easing = Util::EasingFunction(EaseOut, Easing::Quad, currentOffset, static_cast<double>(modeIdx) * Window::Width());
            }
            
            scrolls[modeIdx] -= Mouse::Wheel() * 2.;
            
            if(KeyDown.down() || KeyDown.pressedDuration() > 300ms) {
                scrolls[modeIdx] -= 15.;
            }
            if(KeyUp.down() || KeyUp.pressedDuration() > 300ms) {
                scrolls[modeIdx] += 15.;
            }
            
            if(scrolls[modeIdx] > 0) {
                scrolls[modeIdx] = 0.;
            }
            
            for(auto [i, m] : Indexed(modes)) {
                std::get<1>(m)(scrolls[i]);
            }
            
            if(watch.isRunning()) {
                double t = Min(watch.ms() / 100.0, 1.0);
                
                currentOffset =  easing(t);
                
                if(t == 1.0) {
                    watch.reset();
                }
            }
            
        }
        
        void updateS3DLicense(double& offsetY) {
        }
        
        void updateMotoyaLicense(double& offsetY) {
            auto r = licenseFont(motoyaLicense).region().movedBy(120, 150 + offsetY);
            
            if(r.bottomCenter().y < Window::Height()) {
                offsetY += Window::Height() - r.bottomCenter().y;
            }
        }
        void updateAuthors(double& offsetY) {
        }
        
        void drawS3DLicense(double offsetX, double offsetY) const {
            titleFont(U"Siv3D").draw(120 + offsetX, 80);
            licenseFont(S3dLicense).draw(120 + offsetX, 150);
        }
        
        void drawMotoyaLicense(double offsetX, double offsetY) const {
            titleFont(U"Motoya Font (MTLmr3m)").draw(120 + offsetX, 80 + offsetY);
            licenseFont(motoyaLicense).draw(120 + offsetX, 150 + offsetY);
        }
        void drawAuthors(double offsetX, double offsetY) const {
            titleFont(U"Authors").draw(120 + offsetX, 80);
            authorsFont(Authors).draw(120 + offsetX, 150);
        }
        
        
        void draw() const override{
            leftArrow.draw(16., 36.);
            
            if(modeIdx != 0 ){
                leftArrow.draw(16., Window::Height() / 2);
            }
            
            if (modeIdx != modes.size() - 1 ) {
                rightArrow.draw(Window::Width() - 16., Window::Height() / 2);

            }
            
            Graphics2D::SetTransformLocal(Mat3x2::Translate(-currentOffset, 0));

            for(auto [i, m] : Indexed(modes)) {
                std::get<2>(m)(i * Window::Width(), scrolls[i]);
            }
            
            Graphics2D::SetTransformLocal(Mat3x2::Translate(0, 0));

        }
    };
}
