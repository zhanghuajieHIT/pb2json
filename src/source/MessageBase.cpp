#include "MessageBase.h"

bool MessageBase::getMessageFromProto(const string &proto_path)
{
    string err_string;
    try
    {
        ifstream ifs(proto_path);
        if (!ifs.is_open())
        {
            err_string = "Open file " + proto_path + "failed!";
            cerr << err_string << endl;
            return false;
        }

        int bracket_flag = 0;
        string msg_tmp = "";
        string line_str;
        while(getline(ifs, line_str))
        {
            setMessageList(line_str, bracket_flag, msg_tmp);
        }

        ifs.close();
    }
    catch(exception e)
    {
        err_string = e.what();
        cerr << err_string << endl;
        return false;
    }
}


void MessageBase::setMessageList(const string &line_str, int &bracket_flag, string &msg_tmp)
{
    string usefulStr = getUsefulStr(line_str);
    if ((usefulStr.empty()) || usefulStr.substr(0, 7) == "package" || (usefulStr.substr(0, 6) == "syntax"))
    {
        return;
    }

    if ("{" == usefulStr)
    {
        backet_flag++;
    }

    if ("}" == usefulStr)
    {
        backet_flag--;
    }

    msg_tmp = msg_tmp + usefulStr + '\n';
    if ((0 == bracket_flag) && (!msg_tmp.empty()) && (usefulStr.substr(0, 7) != "message"))
    {
        message_list_.push_back(msg_tmp);
        msg_tmp = "";
    }
}

string MessageBase::getUsefulStr(const string &line_str)
{
    string str_without_blank(line_str);
    auto pos = line_str.find_first_of("//");
    if (string::npos == pos)
    {
        trimBlank(str_without_blank);
        return str_without_blank;
    }

    str_without_blank = line_str.substr(0, pos);
    trimBlank(str_without_blank);
    return str_without_blank;
}

string MessageBase::getOneMessage(unsigned int list_index)
{
    if (message_list_.size() > list_index)
    {
        auto list_begin = message_list_.begin();
        for (int i = 0; i < list_index; ++i)
        {
            ++list_index;
        }
        return *list_begin;
    }
    return "";
}

void MessageBase::trimBlank(string &src_str)
{
    if (!src_str.empty())
    {
        src_str.erase(0, src_str.find_first_not_of("\r\n\t"));
        src_str.erase(src_str.find_last_not_of("\r\n\t") + 1);
    }
}

