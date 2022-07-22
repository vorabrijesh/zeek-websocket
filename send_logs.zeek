
event zeek_init()
{
    Log::add_filter(Conn::LOG, [
        $name = "websocket-conn",
        $writer = Log::WRITER_WEBSOCKETWRITER
    ]);
}