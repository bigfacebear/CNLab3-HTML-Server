//
// Created by 赵前程 on 30/04/2017.
//

#ifndef WEBSERVER_RESPONSE_H
#define WEBSERVER_RESPONSE_H

#include <iostream>
#include <fstream>
#include <cstring>

#include "Request.h"

using namespace std;
class Response {
public:
    Response(){};

    void responseGET(const std::string &filename, std::string &res) {

        std::string path = "../res" + filename;

        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            res = "HTTP/1.1 404 Not Found\r\n";
            res += "Content-Type: text/html; charset=utf-8;\r\n";
            file.close();
            file.open("../res/NotFound.html", std::ios::in | std::ios::binary);
        }
        else {
            res = "HTTP/1.1 200 OK\r\n";
            res += "Content-Type: ";
            std::string type = "";
            for (int i = filename.length() - 1; i >= 0; i--) {
                if (filename[i] == '.') {
                    type = filename.substr(i + 1);
                    break;
                }
            }
            if (type == "txt") {
                res += "text/plain; charset=utf-8;\r\n";
            }
            else if (type == "html") {
                res += "text/html; charset=utf-8;\r\n";
            }
            else if (type == "jpg") {
                res += "image/jpeg;\r\n";
            }
            else {
                res += "text/plain; charset=utf-8;\r\n";
            }
        }

        res += "Content-Length: ";
        file.seekg(0, std::ios::end);
        int end = (int)file.tellg();
        file.seekg(0, std::ios::beg);
        int beg = (int)file.tellg();
        res += std::to_string(end - beg) + "\r\n";

        res += "\r\n";

        char buf[65536];
        file.read(buf, 65536);
        std::string body(buf, end-beg);
        res += body;
    }

    void responsePOST(Request& request, string& res) {
        ifstream file;
        if (request.PATH != "/dopost") {
            res = "HTTP/1.1 404 Not Found \r\n";
            res += "Content-Type: text/html; charset=utf-8;\r\n";
            file.open("../res/NotFound.html", ios::in | ios::binary);

            res += "Content-Length: ";
            file.seekg(0, ios::end);
            auto end = file.tellg();
            file.seekg(0, ios::beg);
            auto beg = file.tellg();

            res += to_string(end - beg) + "\r\n";

            res += "\r\n";

            char buf[65536];
            file.read(buf, 65536);
            string body(buf, end-beg);
            res += body;
        } else {
            string message;
            cout << request.bodyDict["login"] << ":" << request.bodyDict["pass"] << endl;
            if (request.bodyDict["login"] == "3140105594" && request.bodyDict["pass"] == "5594")
            {
                message = "登录成功";
            }else{
                message = "登录失败";
            }

            res = "HTTP/1.1 200 OK \r\n";
            res += "Content-Type: text/html; charset=utf-8;\r\n";

            string html = "<html><body>" + message + "</body></html>";
            res += "Content-Length: ";
            res += to_string(html.size()) + "\r\n";
            res += "\r\n";

            res += html;

            cout << "=========" << endl;
            cout << html.size() << endl;
            cout << res << endl;
            cout << "=========" << endl;
        }
    }
private:
};

#endif //WEBSERVER_RESPONSE_H
