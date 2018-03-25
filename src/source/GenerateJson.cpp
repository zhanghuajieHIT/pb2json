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
    if (regex_match(line_str, match_result, regex("(^[a-zA-Z0-9_]+)+([a-zA-Z0-9_]+) *= *([0-9a-zA-Z]*)*; *")))
    {
        const string proto_value = match_result[3];
        if (!isProtoOriginalType(match_result[1]) && !isEnumType(match_result[1]))
        {
            msg_json[match_result[2]] = match_result[1];
        }
        else
        {
            if (isDigitType(match_result[1]))
            {
                msg_json[match_result[2]] = atoi(proto_value.c_str());
            }
            else if (isEnumType(match_result[1]))
            {
                if (regex_match(proto_value, regex("[0-9]+")))
                {
                    msg_json[match_result[2]] = atoi(proto_value.c_str());
                }
                else
                {
                    auto enum_key = enum_json_map_find(match_result[1]);
                    if (enum_key != enum_json_map_.end())
                    {
                        auto enum_value = (enum_key->second)[match_result[1]][match_result[3]];
                        msg_json[match_result[2] = enum_value;
                    }
                }
            }
            else
            {
                msg_json[match_result[2]] = match_result[3];
            }
        }
    }
    else if (regex_match(line_str, match_result, regex("^repeated + ([a-zA-Z0-9_]+) +([a-zA-Z0-9_]+) *= *([0-9a-zA-Z]*) *; *")))
    {
        repeated_msg_map_.insert(make_pair(match_result[1], match_result[2]));
        msg_json[match_result[2]] = match_result[1];
        return;
    }
    else if (regex_match(line_str, regex("(^[a-zA-Z0-9_]+) + *= *([0-9a-zA-Z_]*) *;*")))
    {
        return;
    }
    else
    {
        cout << line_str << endl;
        cout << "the note is wrong!" << endl;
        return;
    }
}


void GenerateJson::setEnumJsonMap(const string &msg_str)
{
    string line_str;
    string enum_name;
    istringstream stream_str(msg_str);
    json enum_josn;
    json enum_json_value;
    bool flag = false;
    while(getline(stream_str, line_str))
    {
        std::smatch match_result;
        if ("enum" == line_str.substr(0, 4))
        {
            enum_name = getName(line_str, 4);
            flag = true;
        }
        else if (regex_match(line_str, match_result, regex("(^[a-zA-Z0-9_]+) + *= *([0-9a-zA-Z_]*) *; *")))
        {
            const string proto_value = match_result[2];
            enum_json_value[match_result[1]] = atoi(proto_value.c_str());
        }
    }

    if (flag)
    {
        enum_json[enum_name] = enum_json_value;
        enum_json_map_.insert(make_pair(enum_name, enum_json));
    }
}

string GenerateJson::getName(const string &line_str, string::size_type start)
{
    for (; start < line_str.length(); ++start)
    {
        if (!isblank(line_str[start]))
        {
            break;
        }
    }

    auto end = line_str.length() - 1;
    for (; end > start; --end)
    {
        if (line_str[end] == "{")
        {
            continue;
        }
        if (!isblank(line_str[end]))
        {
            break;
        }
    }
    return line_str.substr(start, end - start + 1);
}

bool GenerateJson::isProtoOriginalType(const string &proto_type)
{
    return (proto_type == "int32" || proto_type == "unit32" || proto_type == "sint32" || proto_type == "fixed32" ||
            proto_type == "sfixed32" || proto_type == "int64" || proto_type == "uint64" || proto_type == "sint64" ||
            proto_type == "fixed64" || proto_type == "sfixed64" || proto_type == "double" || proto_type == "float" ||
            proto_type == "bool" || proto_type == "string" || proto_type == "bytes");
}

bool GenerateJson::isDigitType(const string &proto_type)
{
    return (proto_type == "int32" || proto_type == "unit32" || proto_type == "sint32" || proto_type == "fixed32" ||
            proto_type == "sfixed32" || proto_type == "double" || proto_type == "float");
}

bool GenerateJson::isEnumType(const string &proto_type)
{
    return (enum_json_map_.find(proto_type) != enum_json_map_.end());
}

