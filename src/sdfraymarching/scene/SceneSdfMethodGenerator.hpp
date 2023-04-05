#ifndef SDFRAYMARCHINGSCENESDFMETHODGENERATOR_HPP
#define SDFRAYMARCHINGSCENESDFMETHODGENERATOR_HPP

#include <string>

class Scene;

namespace SceneSdfMethodGenerator {
    std::string generateSceneSdfMethod(const Scene* scene);
}


#endif
