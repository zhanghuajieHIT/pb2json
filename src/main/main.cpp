#include <iostream>
#include "MessageBase.h"
#include "PbToJson.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (1 == argc)
    {
        cout << "Please enter the file!" << endl;
        cerr << "Usage:" << argv[0] << file << endl;
        return -1;
    }

    PbToJson pb2json;
    pb2json.parseNote(argv[1]);
    pb2json.getMessageFromProto("proto_tmp.proto");
    pb2json.getJsonFromMsgList();
    pb2json.createJson();
    pb2json.createJsonFile(argv[2], pb2json.getJsonResult());
    return 0;
}
