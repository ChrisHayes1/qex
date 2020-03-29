CC=g++

qexmake: oSpool.cpp oHashJoin.cpp static.cpp oFScan.cpp oStdOp.cpp
	$(CC) -o qex qex.cpp oSpool.cpp oHashJoin.cpp static.cpp oFScan.cpp oStdOp.cpp


gen: gen_data.cpp
	$(CC) -o gen_data gen_data.cpp

read: read_data.cpp
	$(CC) -o read_data read_data.cpp


clean:
	find . -type f -executable -exec rm '{}' \;