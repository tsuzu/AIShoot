#pragma once

namespace Util {
    Vec2 unitVector(const Vec2& v) {
        double len = sqrt(v.x * v.x + v.y * v.y);
        
        return Vec2{v.x / len, v.y / len};
    }
    
    template<typename T>
    std::function<T(double)> EasingFunction(T (*easeFunc)(double f(double), const T& , const T& , const double), double (*f)(double), T from, T to) {
        return [=](double t) {
            return easeFunc(f, from, to, t);
        };
    }
    
    template<typename T>
    std::function<std::pair<T, bool>()>
    EasingFunctionWithTime(
                           T (*easeFunc)(double f(double), const T& , const T& , const double),
                           double (*f)(double),
                           T from,
                           T to,
                           Milliseconds ms,
                           bool backToFrom = false
                           ) {
        auto ef = EasingFunction(easeFunc, f, from, to);
        
        Stopwatch watch;
        return [=, watch = std::move(watch)]() mutable {
            if(!watch.isRunning()) {
                watch.start();
            }
            
            if(ms.count() == 0) {
                return std::make_pair(to, true);
            }
            
            double t = Min(static_cast<double>(watch.ms()) / static_cast<double>(ms.count()), 1.);

            
            auto res = ef(backToFrom ? Abs(t * 2 - 1.0) : t);

            return std::make_pair(res, t >= 1.0);
        };
    }
    
    constexpr double Unspecified = std::numeric_limits<double>::max();
    
    TextureRegion Fit(const TextureRegion& texture, double width, double height, bool scaleUp = true)
    {
        if (!scaleUp)
        {
            width    = std::min<double>(width, texture.size.x);
            height    = std::min<double>(height, texture.size.y);
        }
        
        const double w = texture.size.x;
        const double h = texture.size.y;
        double ws = width / w;    // 何% scalingするか
        double hs = height / h;
        
        double targetWidth, targetHeight;
        
        if (ws < hs)
        {
            targetWidth = width;
            targetHeight = h * ws;
        }
        else
        {
            targetWidth = w * hs;
            targetHeight = height;
        }
        
        TextureRegion result = texture;
        
        result.size = Float2(targetWidth, targetHeight);
        
        return result;
    }
}
