#include "PbToJson.h"

void PbToJson::createJson()
{
    json message_json_input = findJsonHead();
    string json_result_tmp = replaceJson(message_json_input);
    json_result_ = strDump(json_result_tmp);
}

json PbToJson::findJsonHead()
{
    return message_json_map_["Message"]["Message"];
}

string PbToJson::replaceJson(const json &src_json)
{
    string src_str = src_json.dump();
    for (auto iter  = src_json.begin(); iter != src_json.end(); ++iter)
    {
        if (!(*iter).is_string())
        {
            continue;
        }
        auto msg_index = message_json_map_.find(*iter);
        if (message_json_map_.end() != msg_index)
        {
            json map_value_json = (msg_index->second)[msg_index->first];
            string replace_str = replaceJson(map_value_json);
            int start_pos = src_str.find((*iter).get<string>()) - 1;
            int rep_len = (*iter).get<string>().length() + 2;
            src_str.replace(start_pos, rep_len, replace_str);

            if (repeated_msg_map_.find(*iter) != repeated_msg_map_.end())
            {
                objectToArray(stc_str, repeated_msg_map_[*iter]);
            }
        }
    }
    return src_str;
}

void PbToJson::objectToArray(string &src_object, const string json_key)
{
    json object_json = json::parse(src_object);
    json array_json;
    array_json[0] = object_json[json_key];
    object_json[json_key] = array_json;
    src_object = object_json.dump();
}

bool PbToJson::keyIsJsonValue(cont string &json_key)
{
    for (auto iter = message_json_map_.begin(); iter != message_json_map_.end(); ++iter)
    {
        json child_json = (iter->second)[iter->first];
        for (auto i = child_json.begin(); i != child_json.end(); ++i)
        {
            if (((*i).is_string()) && (*i == json_key))
            {
                return ture;
            }
        }
    }
    return false;
}

string PbToJson::getJsonResult()
{
    return json_result_;
}

string PbToJson::strDump(const string &str_input)
{
    json str_json = json::parse(str_input);
    return str_json.dump(4);
}
