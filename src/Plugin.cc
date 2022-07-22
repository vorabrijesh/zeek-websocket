
#include "Plugin.h"

namespace plugin { namespace Ennetix_Websocket { Plugin plugin; } }

using namespace plugin::Ennetix_Websocket;

zeek::plugin::Configuration Plugin::Configure()
	{
		AddComponent(new zeek::logging::Component("WebsocketWriter",
        zeek::logging::writer::WebsocketWriter::Instantiate));

	zeek::plugin::Configuration config;
	config.name = "Ennetix::Websocket";
	config.description = "Send logs via simple-websocket-server, developed by Ennetix";
	config.version.major = 0;
	config.version.minor = 1;
	config.version.patch = 0;
	return config;
	}
