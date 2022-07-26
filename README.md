
Ennetix::Websocket
=================================
This zeek plugin uses Simple-websocket-server: https://gitlab.com/eidheim/Simple-WebSocket-Server library to send the logs. 

MAke sure to install the Simple-websocket-library.

Steps to configuring and installing
```
$ ./configure --with-websocket=/usr/local
```
Replace path with the Simple-websocket-library path.

```
$ cd build/
$ cmake ..
$ make
$ make install
$ zeek -N
```
With the last command, you can see that the plugin included in the list.

Also make sure that in /usr/local/incude the folder named simple-websocket-server is present.

Then collect the packets using tcpdump. 
```
$ sudo tcpdump -s 0 -w get.trace
```
Right now, the zeek-server just send hello msgs to and fro but can be implemented to logs also.

Run:
```
$ Zeek -C -r get.trace send_logs.zeek
```

Also shift the demo_client.cpp to simple-websocket-server folder and run 
```
$ g++ demo_client.cpp -o demo_client -L/usr/lib -lssl -lcrypto 
$ ./demo_client
```
