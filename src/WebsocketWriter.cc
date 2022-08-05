#include "WebsocketWriter.h"

using namespace zeek::logging;
using namespace writer;
using namespace std;

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;


// The Constructor is called once for each log filter that uses this log writer.
WebsocketWriter::WebsocketWriter(WriterFrontend *frontend)
    : WriterBackend(frontend), formatter(NULL) {

}

WebsocketWriter::~WebsocketWriter() {
  // Cleanup must happen in DoFinish, not in the destructor
}

bool WebsocketWriter::DoInit(const WriterInfo &info, int num_fields, const threading::Field *const *fields) {
  threading::formatter::JSON::TimeFormat tf = threading::formatter::JSON::TS_EPOCH;
  formatter = new threading::formatter::JSON(this, tf);
  only_once = 0;
  input_log= "";
  return true;
}

bool WebsocketWriter::DoFinish(double network_time) {
	return true;
}


bool WebsocketWriter::DoWrite(int num_fields, const threading::Field *const *fields,
                          threading::Value **vals) {
    ODesc buff;
    buff.Clear();
    
    formatter->Describe(&buff, num_fields, fields, vals);
    // send the formatted log entry to kafka
    const char *raw = (const char *)buff.Bytes();
    std::cout<<const_cast<char *>(raw)<<endl;
    std::string s(const_cast<char *>(raw));
    input_log += s + "\n";			
   

    server.config.port = 8080;
    auto &echo = server.endpoint["^/echo/?$"];
    echo.on_message = [this](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {

    auto out_message = make_shared<string>(input_log); //in_message->string();
    // cout << "Server: Message received: \"" << out_message << "\" from " << connection.get() << endl;

    // cout << "Server: Sending message \"" << out_message << "\" to " << connection.get() << endl;
    
    // connection->send is an asynchronous function
    connection->send(input_log, [](const SimpleWeb::error_code &ec) {
        if(ec) {
        cout << "Server: Error sending message. " <<
            // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
            "Error: " << ec << ", error message: " << ec.message() << endl;
        }
    });
    input_log = "";
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

  if (only_once==0)		// start the server thread only once.
  {
	only_once = 1;
	
	promise<unsigned short> server_port;
	std::thread server_thread( [this, &server_port]() {
		// Start server
		server.start([&server_port](unsigned short port) {
			server_port.set_value(port);
		});
	});
	cout << "Server listening on port " << server_port.get_future().get() << endl << endl;
	
	// server_thread.join();
	server_thread.detach();
  }
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