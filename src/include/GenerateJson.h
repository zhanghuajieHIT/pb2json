#ifndef GENERATEJSON_H_INCLUDED
#define GENERATEJSON_H_INCLUDED

#include <iostream>
#include <string>
#include "json.hpp"
#include <map>
#include "MsgBase.h"
using namespace std;
using json = nlohmann::json;

class GenerateJson: public MessageBase
{
public:
    void getJsonFromMsgList();
    bool isProtoOriginalType(const string &proto_type);
    bool isDigitType(const string &proto_type);
    void setJsonMap(const string &msg_str);
    string getName(const string &line_str, string::size_type start);
    void setOneJsonHpp(const string &line_str, json &msg_json);
    bool isEnumType(const string &proto_type);
    void setEnumJsonMap(const string &msg_str);
    void splitFromMultiMsg(const string &multi_msg_str, list<string> &one_msg_list);

protected:
    map<string, json> message_json_map_;
    map<string, json> enum_json_map_;
    map<string, string> repeated_msg_map_;
};



#endif // GENERATEJSON_H_INCLUDED
