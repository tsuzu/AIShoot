# define NO_S3D_USING

#include "MacBundle.h"

#ifdef __APPLE__

#import <Foundation/Foundation.h>

namespace Util {
    s3d::String getResourcesPath() {
        return s3d::Unicode::Widen([[[NSBundle mainBundle] resourcePath]UTF8String]);
    }
}

#endif

