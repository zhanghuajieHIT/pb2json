#include "ParseNote.h"

void ParseNote::parseNote(const string &proto_type)
{
    string err_string;
    try
    {
        ifstream ifs(proto_path);
        if (!ifs.is_open())
        {
            err_string = "Open file " + proto_type + "failed!";
            cerr << err_string << endl;
            return;
        }

        string msg_tmp = "";
        string line_str;
        while (getline(ifs, line_str))
        {
            setInitValue(line_str);
        }
        ifs.close();
        createJsonFile("proto_tmp.proto", parse_note_result_);
    }
    catch(exception e)
    {
        err_string = e.what();
        cerr << err_string << endl;
        return;
    }
}

void ParseNote::setInitValue(const string &line_str)
{
    if (line_str.find("//") == string::npos || line_str.find("=") == string::npos ||
        line_str,find("{") == string::npos || line_str.find("}") == string::npos)
    {
        parse_note_result_ = parse_note_result_ + line_str + '\n';
        return;
    }
    string note_str = getNote(line_str);
    if (note_str.empty())
    {
        return;
    }
    string init_value = getInitValue(note_str);
    if (init_value.empty())
    {
        return;
    }
    string new_line_str = line_str;
    int start_pos = line_str.find("=") + 1;
    int rep_len = line_str.find(";") - start_pos;
    new_line_str.replace(start_pos, rep_len, init_value);
    parse_note_result_ = parse_note_result_ + new_line_str + '\n';
}

string ParseNote::getNote(const string &line_str)
{
    int start_pos = line_str.find("//") + 1;
    int note_len = line_str.length() - start_pos;
    return line_str.substr(start_pos, note_len);
}

string ParseNote::getInitValue(const string &note_str)
{
    auto left_pos = note_str.find("{");
    auto right_pos = note_str.find("}");

    if (left_pos != note_str.rfind("{") || right_pos != note_str.rfind("}"))
    {
        cout << "the note format is wrong!" << endl;
        return "";
    }

    string init_value_tmp = note_str.substr(left_pos + 1, right_pos - left_pos -1);
    std::smatch match_result;
    if (regex_match(init_value_tmp, match_result, regex("(^ *)([a-zA-Z0-9]+)( *)$")))
    {
        return match_result[2];
    }
    return setRandValue(init_value_tmp);
}

string ParseNote::setRandValue(const string &note_str)
{
    std::match match_result;
    if (regex_match(note_str, match_result, regex("(^ *)[0-9]+( *- *)([0-9]+)( *)$")))
    {
        return match_result[2];
    }

    if (regex_match(note_str, match_result, regex("(^ *< *)([0-9]+)( *)$")))
    {
        string match_str = match_result[2];
        int rand_value_int = atoi(match_str.c_str()) - 1;
        char rand_value[11];
        snprintf(rand_value, 11, "%d", rand_value_int);
        return rand_value;
    }

    if (regex_match(note_str, match_result, regex("(^ *> *)([0-9]+)( *)$")))
    {
        string match_str = match_result[2];
        int rand_value_int = atoi(match_str.c_str()) + 1;
        char rand_value[11];
        snprintf(rand_value, 11, "%d", rand_value_int);
        return rand_value;
    }

    if (regex_match(note_str, match_result, regex("(^ *)([0-9 ,]+)( *)$")))
    {
        string match_str = match_result[2];
        return match_str.substr(0, match_str.find(","));
    }

    return "";
}

void ParseNote::createJsonFile(const string &json_path, const string &output_str)
{
    string err_string;
    ofstream fout(json_path);
    if (!fout.is_open())
    {
        err_string  = "Open file " + json_path + "failed!";
        cerr << err_string << endl;
        return;
    }
    fout << output_str;
    fout.close();
}
