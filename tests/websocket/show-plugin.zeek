# @TEST-EXEC: zeek -NN Ennetix::Websocket |sed -e 's/version.*)/version)/g' >output
# @TEST-EXEC: btest-diff output
