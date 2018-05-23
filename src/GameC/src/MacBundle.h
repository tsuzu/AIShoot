#pragma once


#ifdef __APPLE__

#include <Siv3D.hpp>

namespace Util {
    s3d::String getResourcesPath();
}
#else

namespace Util {
	s3d::String getResourcesPath() {
		return U".";
	}
}

#endif
