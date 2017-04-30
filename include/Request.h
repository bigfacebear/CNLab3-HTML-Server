//
// Created by 赵前程 on 30/04/2017.
//

#ifndef WEBSERVER_REQUEST_H
#define WEBSERVER_REQUEST_H

#include <iostream>

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
    }

    std::string METHOD;
    std::string PATH;
    std::string BODY;
private:

};

#endif //WEBSERVER_REQUEST_H
