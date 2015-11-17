#include "parser.h"
using namespace std;
int main() 
{
    Parser parser("scene.json");
    vector <SceneObject *> objs = parser.parse_file() ;
    cout << objs[0] << endl;

    return 0;
}


