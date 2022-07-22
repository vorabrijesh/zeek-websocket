
#pragma once

#include <zeek/plugin/Plugin.h>
#include "WebsocketWriter.h"

namespace plugin {
namespace Ennetix_Websocket {

class Plugin : public zeek::plugin::Plugin
{
protected:
	// Overridden from zeek::plugin::Plugin.
	zeek::plugin::Configuration Configure() override;
};

extern Plugin plugin;

}
}
