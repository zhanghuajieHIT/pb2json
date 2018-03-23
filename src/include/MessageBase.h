#ifndef MSG_H
#define MSG_H

#include <iostream>
#include "json.hpp"
#include <string>
#include <list>
#include <fstream>

using namespace std;
using json = nlohmann::json

class MessageBase
{
public:
    bool getMessageFromProto(const string &proto_path);
    string getUsefulStr(const string &line_str);
    void getInitPara();
    void setMessageList(const string &line_str, int &backet_flag, string &msg_tmp);
    string getOneMessage(unsigned int list_index);
    void trimBlank(string &str_str);

private:
    list<string> message_list_;
};


#endif
