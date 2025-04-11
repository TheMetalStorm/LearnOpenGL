#include "SimiEng/SimiEng.h"


namespace SimiEng {
    static bool gladInitialized = false;
    SIMIENG_API bool Init(GLADloadproc loadProc) {
        if (!gladInitialized) {
            if (!gladLoadGLLoader(loadProc)) {
                return false;
            }
            gladInitialized = true;
        }
        return true;
    }
}
