#include "gateway_client.h"
#include "rest_client.h"
#include <json.hpp>
#include <iostream>

int quit = 0;
concordpp::rest_client *d_rest;

void on_message(nlohmann::json data) {
    if(data["content"] == "test me") {
        d_rest->create_message(data["channel_id"], "This is (not) a test");
    } else if(data["content"] == "shutdown") {
        d_rest->create_message(data["channel_id"], "zzz", [](int code, nlohmann::json data) { std::cout << data.dump() << std::endl; });
        quit++;
    }
}

int main(int argc, char* argv[]) {
    concordpp::gateway_client *d_gateway = new concordpp::gateway_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    d_rest = new concordpp::rest_client("MzAwMzg2MTQ2OTQ3NDMyNDQ4.DApDZg.W5G01s0dGHJV9NgIVETQCZ3c_WE");
    d_gateway->add_callback("MESSAGE_CREATE", on_message);
    d_gateway->add_callback("MESSAGE_CREATE", [](nlohmann::json data){
        if(data["content"] == "lambda") {
            d_rest->create_message(data["channel_id"], "Lambda test done.");
        }
    });
    while(quit == 0) {}
    delete d_gateway;
    delete d_rest;
}
