#include "parser.h"

int main()
{
    Parser parser("scene.json");
    std::vector<SceneObject *> objs = parser.parse_file();
    std::cout << objs[0] << std::endl;
    return 0;
}
