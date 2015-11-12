#include "dist/json/json.h"
#include "dist/json/json-forwards.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void parse_file(string filename) {
    
    ifstream contents;
    contents.open(filename);
    Json::Value root;
    Json::Reader reader;
    bool json_parse_successful = reader.parse(contents, root);

    if (json_parse_successful) {
        cout << "Json Parsing Success" <<endl;
        Json::Value::Members json_members = root.getMemberNames();

        for (string member : json_members) {
            cout << member << " " << root[member] << " " << root[member].type() << endl;
        }

        int i = root["def"].asInt();
        cout << i << endl;
    }
    
}


int main() 
{
    parse_file("example.json");

    return 0;
}


