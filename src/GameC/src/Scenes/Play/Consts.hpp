
namespace Scenes::Play {
    const String resourceDir = Util::getResourcesPath();
    
    const s3d::String GameTitle = U"愛Shoot!";
    
    const s3d::String goodSEPath = resourceDir + U"/assets/good_se.wav";
    const s3d::String badSEPath = resourceDir  +U"/assets/bad_se.wav";
    const s3d::String mainBGMPath = resourceDir +U"/assets/mainbgm.wav";
    
    const s3d::String backgroundTitlePath = resourceDir +U"/assets/background_title.png";
    
    constexpr int Timelimit = 90; // in seconds
    const s3d::String heartBarIconPath = resourceDir + U"/assets/heart_sample.png";
    const s3d::String flowerBarIconPath = resourceDir + U"/assets/flower_sample.png";
    const s3d::String heroPath = resourceDir + U"/assets/hero.png";
    const s3d::String heroFrontPath = resourceDir + U"/assets/hero_front.png";
    const s3d::String heroCutPath = resourceDir +  U"/assets/hero_cut.png";
    const s3d::String heroinePath = resourceDir + U"/assets/aiko.png";
    const s3d::String villainPath = resourceDir + U"/assets/kinpatsu.png";
    const s3d::String villainSmallPath = resourceDir + U"/assets/kinpatsu_small.png";
    const s3d::String backgroundRoadPath = resourceDir + U"/assets/background_road.jpg";
    const s3d::String backgroundUpperPath = resourceDir + U"/assets/background_upper.jpg";
    const s3d::String grassPath = resourceDir + U"/assets/grass.png";
    const s3d::String obstaclePath = resourceDir + U"/assets/obstacle.png";
    
    const s3d::String descriptionPath = resourceDir + U"/assets/screen_shot.png";
    const s3d::String prologuePath = resourceDir + U"/assets/prologue.png";
    
    const s3d::String ikemenEndPath = resourceDir + U"/assets/ikemen_end.jpg";
    const s3d::String trueEndPath = resourceDir + U"/assets/true_end.jpg";
    const s3d::String badEndPath = resourceDir + U"/assets/bad_end.png";
    const s3d::String gameOverPath = resourceDir + U"/assets/game_over.jpg";
    
    const s3d::String fontPMPlusPath = resourceDir + U"/assets/PixelMplus10-Regular.ttf";
    
    const s3d::String fontMTLPath = resourceDir + U"/assets/MTLmr3m.ttf";
    
    const Rect bulletRect = RectF(0, 0, 20, 5).movedBy(75 - 20, -2.5);
    
    const Array<String> GameDescription{
        U"ゲーム説明をします。\nこれがゲーム画面です。",
        U"画面内をクリックすると、\n下のプレイヤーからその方向へ弾が飛んでいきます。",
        U"画面内を流れてくる物は3種類あります。\nハート、花、お邪魔です。",
        U"ハートと花をとると右上のそれぞれのバーが増加します。\nお邪魔をとると両方のバーが大きく減少します。",
        U"また、バーは時間の経過とともに少しずつ減少するので注意してください。\n左上のイケメンが愛子さんに到達するとゲーム終了です。",
        U"頑張って右上のバーが線を超えられるようにしましょう。\nそれではゲームスタートです。健闘を祈ります！",
    };
    
    const Array<std::function<void(bool, TextureRegion, Vec2)>> GameDescriptionDrawing {
        [](bool, TextureRegion, Vec2){},
        [](bool isIndexChanged, TextureRegion r, Vec2 pos){
            r.region().movedBy(r.size / -2).movedBy(pos).draw(ColorF(0., 0., 0., 0.6));
            
            const double radius = 40.;
            const auto center = Vec2{502, 287};
            
            pos -= r.region().size / 2;
            
            
            auto scale = r.size;
            
            r.uvRect.top = (center.y - radius - pos.y) / scale.y;
            r.uvRect.bottom = r.uvRect.top + radius * 2. / scale.y;
            r.uvRect.left = (center.x - radius - pos.x) / scale.x;
            r.uvRect.right = r.uvRect.left + radius * 2 / scale.x;
            
            
            Circle(center, radius)(r).draw();
        },
        [](bool, TextureRegion r, Vec2 pos){
            static Font font(40, resourceDir + U"/assets/MTLmr3m.ttf");
            const double iconHeight = 70.;
            
            static auto heart = Util::Fit(Texture(heartBarIconPath), Util::Unspecified, iconHeight);
            static auto flower = Util::Fit(Texture(flowerBarIconPath), Util::Unspecified, iconHeight);
            static auto obstacle = Util::Fit(Texture(obstaclePath), Util::Unspecified, iconHeight);
            
            const Vec2 size{210, 210};
            
            auto windowSize = Window::Size();
            windowSize.y -= 100. - 5.;
            
            auto leftTop = (windowSize - size) / 2.;
            
            r.region().movedBy(r.size / -2).movedBy(pos).draw(ColorF(0., 0., 0., 0.6));
            
            {
                const auto r = heart.draw(leftTop);
                const auto text = font(U"ハート");
                
                text.drawAt(r.rightCenter().x + text.region().w / 2., r.rightCenter().y, Color(208,45,53));
                
                leftTop.y = r.bottomCenter().y;
            }
            
            {
                const auto r = flower.draw(leftTop);
                const auto text = font(U"花");
                
                text.drawAt(r.rightCenter().x + text.region().w / 2., r.rightCenter().y, Color(239,182,63));
                
                leftTop.y = r.bottomCenter().y;
            }
            
            {
                const auto r = obstacle.draw(leftTop);
                const auto text = font(U"お邪魔");
                
                text.drawAt(r.rightCenter().x + text.region().w / 2., r.rightCenter().y, Color(205,187,104));
                
                leftTop.y = r.bottomCenter().y;
            }
        },
        [](bool, TextureRegion r, Vec2 pos){
            r.region().movedBy(r.size / -2).movedBy(pos).draw(ColorF(0., 0., 0., 0.6));
            
            auto rect = RoundRect(438, 2, 295, 120, 15);
            
            pos -= r.region().size / 2;
            
            auto scale = r.size;
            
            r.uvRect.top = (rect.y - pos.y) / scale.y;
            r.uvRect.bottom = r.uvRect.top + rect.h / scale.y;
            r.uvRect.left = (rect.x - pos.x) / scale.x;
            r.uvRect.right = r.uvRect.left + rect.w / scale.x;
            
            rect(r).draw();
        },
        [](bool, TextureRegion, Vec2){},
        [](bool, TextureRegion, Vec2){},
    };
    
    const Array<String> TrueEndStory = {
        U"愛も花も十分に集まった！\n愛子さんに告白するぞ！",
        U"しゅうと「僕と付き合ってください・・・!」",
        U"愛子「は、はい・・・！」",
        U"無事結ばれましたとさ。\nめでたしめでたし。",
    };
    const Array<String> IkemenEndStory = {
        U"愛は十分に集まったが花は集め切れなかった・・・。",
        U"イケメン「俺と付き合ってくれ！」",
        U"愛子「は、はい」",
        U"しゅうと「・・・。」\nイケメンと愛子さんが結ばれてしまった・・・。",
    };
    
    const Array<String> BadEndStory = {
        U"花は十分に集まったが愛は集め切れなかった・・・。",
        U"しゅうと「あれ・・・僕はどうして花束を集めていたんだろう・・・」",
    };
    
    const Array<String> GameOverStory = {
        U"愛も花も十分に集めることができなかった・・・。",
    };
    
    const String HeroIntroduction = U"名前: しゅうと\n主人公。\n幼馴染の愛子さんのことが好きで告白しようとしている。";
    const String HeroineIntroduction = U"名前: 愛子\nヒロイン。\n幼馴染の\"しゅうと\"のことはあまり意識していない。";
    const String VillainIntroduction = U"名前: 不明(呼称: イケメン)\nモブ(?)キャラ。\n意外とイイヤツらしい。";
    
    const std::map<EndKind, std::pair<String, Array<String>>> Ends { // {path, [story]}
        {EndKind::TrueEnd, {trueEndPath, TrueEndStory}},
        {EndKind::IkemenEnd, {ikemenEndPath, IkemenEndStory}},
        {EndKind::BadEnd, {badEndPath, BadEndStory}},
        {EndKind::GameOver, {gameOverPath, GameOverStory}},
    };
    
    
    
}
