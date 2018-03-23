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

}
