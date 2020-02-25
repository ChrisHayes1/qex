CC=g++

qexmake: oGenData.cpp oAddData.cpp oMultData.cpp qex.cpp
	$(CC) -o qex qex.cpp oGenData.cpp oAddData.cpp oMultData.cpp


clean:
	rm ./*.o