#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include <iostream>
#include <string>

using namespace std;


int main() 
{
    string file = "{\"abc\":\"baloney\", \"def\":5}";
    Json::Value root;
    Json::Reader reader;
    bool json_parse_successful = reader.parse(file, root);

    if (json_parse_successful) {
        cout << "Json Parsing Success" <<endl;
        Json::Value::Members json_members = root.getMemberNames();

        for (string member : json_members) {
            cout << member << " " << root[member] << " " << root[member].type() << endl;
        }

        int i = root["def"].asInt();
    }



    return 0;
}


