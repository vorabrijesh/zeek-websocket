#ifndef ZEEK_PLUGIN_KAFKA_WebsocketWriter_H
#define ZEEK_PLUGIN_KAFKA_WebsocketWriter_H
#include <simple-websocket-server/server_ws.hpp>
#include <simple-websocket-server/client_ws.hpp>
#include <map>
#include <string>
#include <bits/stdc++.h>
#include <zeek/Desc.h>
#include <zeek/logging/WriterBackend.h>
#include <zeek/threading/formatters/JSON.h>
#include <zeek/threading/Formatter.h>
#include <future>
#include <pthread.h>
#include "websocket.bif.h"

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
namespace zeek::logging::writer {

/**
 * A logging writer that sends data to a Kafka broker.
 */
class WebsocketWriter : public WriterBackend {

public:
    explicit WebsocketWriter(WriterFrontend* frontend);
    ~WebsocketWriter();

    static WriterBackend* Instantiate(WriterFrontend* frontend)
    {
        return new WebsocketWriter(frontend);
    }

protected:
    virtual bool DoInit(const WriterBackend::WriterInfo& info, int num_fields, const threading::Field* const* fields);
    virtual bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals);
    virtual bool DoSetBuf(bool enabled);
    virtual bool DoRotate(const char* rotated_path, double open, double close, bool terminating);
    virtual bool DoFlush(double network_time);
    virtual bool DoFinish(double network_time);
    virtual bool DoHeartbeat(double network_time, double current_time);
private:
    threading::Formatter *formatter;
    int only_once;
    WsServer server;
    std::thread server_thread;
    std::string input_log;
};

}

#endif
