CC=g++

qexmake: oGenData.cpp oAddData.cpp qex.cpp
	$(CC) -o qex qex.cpp oGenData.cpp oAddData.cpp


clean:
	rm ./*.o