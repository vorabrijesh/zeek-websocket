
#include "WebsocketWriter.h"

using namespace zeek::logging;
using namespace writer;
using namespace std;

using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;
using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;

// The Constructor is called once for each log filter that uses this log writer.
WebsocketWriter::WebsocketWriter(WriterFrontend *frontend)
    : WriterBackend(frontend) {

}

WebsocketWriter::~WebsocketWriter() {
  // Cleanup must happen in DoFinish, not in the destructor
}


bool WebsocketWriter::DoInit(const WriterInfo &info, int num_fields, const threading::Field *const *fields) {
  
    WsServer server;
    server.config.port = 8080;
    auto &echo = server.endpoint["^/echo/?$"];

    echo.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {
    auto out_message = in_message->string();

    cout << "Server: Message received: \"" << out_message << "\" from " << connection.get() << endl;

    cout << "Server: Sending message \"" << out_message << "\" to " << connection.get() << endl;

    // connection->send is an asynchronous function
    connection->send(out_message, [](const SimpleWeb::error_code &ec) {
        if(ec) {
        cout << "Server: Error sending message. " <<
            // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
            "Error: " << ec << ", error message: " << ec.message() << endl;
        }
    });

  };

  echo.on_open = [](shared_ptr<WsServer::Connection> connection) {
    cout << "Server: Opened connection " << connection.get() << endl;
  };

  // See RFC 6455 7.4.1. for status codes
  echo.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
    cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
  };

  // Can modify handshake response headers here if needed
  echo.on_handshake = [](shared_ptr<WsServer::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/) {
    return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
  };

  // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
  echo.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
    cout << "Server: Error in connection " << connection.get() << ". "
         << "Error: " << ec << ", error message: " << ec.message() << endl;
  };


  // Start server and receive assigned port when server is listening for requests
  promise<unsigned short> server_port;
  std::thread server_thread([&server, &server_port]() {
    // Start server
    server.start([&server_port](unsigned short port) {
      server_port.set_value(port);
    });
  });
  cout << "Server listening on port " << server_port.get_future().get() << endl
       << endl;


  server_thread.join();
  
  return true;
}

bool WebsocketWriter::DoFinish(double network_time) {
  return true;
}


bool WebsocketWriter::DoWrite(int num_fields, const threading::Field *const *fields,
                          threading::Value **vals) {
          std::cout<<"reached in do write\n";
  return true;
}


bool WebsocketWriter::DoSetBuf(bool enabled) {
  // no change in behavior
  return true;
}


bool WebsocketWriter::DoFlush(double network_time) {
  return true;
}


bool WebsocketWriter::DoRotate(const char *rotated_path, double open, double close,
                           bool terminating) {
  // no need to perform log rotation
  return FinishedRotation();
}

bool WebsocketWriter::DoHeartbeat(double network_time, double current_time) {
  
  return true;
}