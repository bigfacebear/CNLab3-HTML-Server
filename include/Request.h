//
// Created by 赵前程 on 30/04/2017.
//

#ifndef WEBSERVER_REQUEST_H
#define WEBSERVER_REQUEST_H

#include <iostream>
#include <map>

using namespace std;

class Request {
public:
    Request(char *rcv_msg, int len) :METHOD(""), PATH(""), BODY(""){
        if (len < 0) {
            return;
        }
        char buf[4096];
        int index = 0;
        int num;
        // get method
        for (num = 0; index < len; index++, num++) {
            if (rcv_msg[index] == ' ') {
                index++;
                break;
            }
            buf[num] = rcv_msg[index];
        }
        if (num > 0) {
            buf[num] = '\0';
            METHOD = std::string(buf);
        }
        else {
            METHOD = ""; // invalid method
            return;
        }
        // get path
        for (num = 0; index < len; index++, num++) {
            if (rcv_msg[index] == ' ') {
                index++;
                break;
            }
            buf[num] = rcv_msg[index];
        }
        if (num > 0) {
            buf[num] = '\0';
            PATH = std::string(buf);
        }
        else {
            METHOD = ""; // invalid method
            return;
        }
        // get body
        for (; index < len; index++) {
            if (rcv_msg[index] == '\n') {
                if (rcv_msg[index + 2] == '\n') {
                    index += 3;
                    break;
                }
            }
        }
        rcv_msg[len] = '\0';
        BODY = std::string(&rcv_msg[index]);

        parseBody();

    }

    std::string METHOD;
    std::string PATH;
    std::string BODY;
    std::map<std::string, std::string> bodyDict;
private:

    void parseBody() {
        vector<string> parts;
        split(BODY, "&", parts);
        for (auto token:parts) {
            vector<string> keyValue;
            split(token, "=", keyValue);
            bodyDict.emplace(make_pair(keyValue[0], keyValue[1]));
        }
    }

    void split(const string& str, const string& delim, vector<string>& parts) {
        size_t start, end = 0;
        while (end < str.size()) {
            start = end;
            while (start < str.size() && (delim.find(str[start]) != string::npos)) {
                start++;  // skip initial whitespace
            }
            end = start;
            while (end < str.size() && (delim.find(str[end]) == string::npos)) {
                end++; // skip to end of word
            }
            if (end-start != 0) {  // just ignore zero-length strings.
                parts.push_back(string(str, start, end-start));
            }
        }
    }
};

#endif //WEBSERVER_REQUEST_H
