#ifndef PARSE_NOTE_H
#define PARSE_NOTE_H

#include <iostream>
#include <string>
#include <fstream>
#include <regex>
using namespace std;

class ParseNote
{
public:
    void createJsonFile(const string &json_path, const string &output_str);
    void parseNote(const string &proto_path);
    string getInitValue(const string &note_str);
    string getNote(const string &line_str);
    void setInitValue(const string &line_str);
    string setRandValue(const string &note_str);

private:
    string parse_note_result_;
};

#endif // PARSE_NOTE_H
