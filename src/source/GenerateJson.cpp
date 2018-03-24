#include "GenerateJson.h"
#include <regex>
#include <cstdio>
#include <sstream>

void GenerateJson::getJsonFromMsgList()
{
    string multi_msg_str;
    unsigned int list_index = 0;
    for (;;++list_index)
    {
        multi_msg_str = getOneMessage(list_index);
        if (multi_msg_str.empty())
        {
            return;
        }

        list<string> one_msg_list;
        splitFromMultiMsg(multi_msg_str, one_msg_list);
        for (auto iter = one_msg_list.begin(); iter != one_msg_list.end(); ++iter)
        {
            setEnumJsonMap(*iter);
            setJsonMap(*iter);
        }
    }
}

void GenerateJson::splitFromMultiMsg(const string &multi_msg_str, list<string> &one_msg_list)
{
    int msg_layer_num = 0;
    int unuse_back_num = 0;
    string line_str;
    string line_str_last = "";
    list<string>  one_msg_list_tmp;
    istringstream stream_str(multi_msg_str);
    while (getline(stream_str, line_str))
    {
        if ("{" == line_str)
        {
            if ("message" == line_str_last.substr(0, 7))
            {
                ++msg_layer_num;
                one_msg_list_tmp.push_back(line_str_last + '\n' + line_str + '\n');
                line_str_last = line_str;
                continue;
            }
            ++unuse_back_num;
        }
        else if ("}" == line_str)
        {
            if (0 == unuse_back_num)
            {
                --msg_layer_num;
                string back_str = one_msg_list_tmp.back();
                back_str = back_str + "}";
                one_msg_list.push_back(back_str + '\n');
                one_msg_list_tmp.pop_back();
                line_str_last = line_str;
                continue;
            }
            --unuse_back_num;
        }
        if (regex_match(line_str, regex("(^[a-zA-Z0-9_]+)+([a-zA-Z0-9_]+)*=*([a-zA-Z0-9]*) *;*")))
        {
            auto iter = one_msg_list_tmp.begin();
            if (iter != one_msg_list_tmp.end())
            {
                for (int i = 0; i < msg_layer_num; ++i)
                {
                    *iter = *iter + line_str + '\n';
                    ++iter;
                }
            }
        }
        line_str_last = line_str;
    }
}

void GenerateJson::setJsonMap(const string &msg_str)
{
    string line_str;
    string msg_name;
    istringstream stream_str(msg_str);
    json msg_json;
    json msg_json_value;
    bool flag = false;
    while(getline(stream_str, line_str))
    {
        if ("message" == line_str.substr(0 7))
        {
            msg_name = getName(line_str, 7);
            flag = true;
        }
        else if (line_str.find("=") != string::npos)
        {
            setOneJsonHpp(line_str, msg_json_value);
        }
    }

    if (flag)
    {
        msg_json[msg_name] = msg_json_value;
        message_json_map_.insert(make_pair(msg_name, msg_json));
    }
}

void GenerateJson::setOneJsonHpp(const string &line_str, json &msg_json)
{
    std::smatch match_result;

}
