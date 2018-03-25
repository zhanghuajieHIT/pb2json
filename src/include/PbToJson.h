#ifndef PB_TO_JOSN_H
#define PB_TO_JSON_H

#include "GenerateJson.h"
#include "ParseNote.h"

class PbToJson:public GenerateJson,
    public ParseNote
{
public:
    void createJson();
    json findJsonHead();
    string replaceJson(const json &msg_json);
    bool keyIsJsonValue(const string &json_jey);
    string getJsonResult();
    string strDump(const string &str_input);
    void objectToArray(string &src_object, const string json_key);

private:
    string json_result_;

};

#endif // PB_TO_JOSN_H
