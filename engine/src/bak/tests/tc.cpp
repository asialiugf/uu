#include <uWS/uWS.h>
#include <unistd.h>

#pragma comment(lib, "uWS.lib")

int main()
{
    uWS::Hub h;

    // 客户端连上后发送hello
    h.onConnection([](uWS::WebSocket<uWS::CLIENT> *ws, uWS::HttpRequest req) {
        while(1) {
            ws->send("--client1111--");
            //sleep(1);
            usleep(1000000);
 			break;
        }

    });

    // 客户端打印接收到的消息
    h.onMessage([](uWS::WebSocket<uWS::CLIENT> *ws, char *message, size_t length, uWS::OpCode opCode) {
        char tmp[256];
        memcpy(tmp, message, length);
        tmp[length] = 0;
        printf("Client receive: %s\n", tmp);
        //ws->send(message, length, opCode);
        //usleep(1000000);

        //ws->close();
    });

    h.connect("ws://localhost:3003");

    h.run();
}
