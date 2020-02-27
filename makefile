CC=g++

qexmake: oGenData.cpp oAddData.cpp oMultData.cpp oManyToMany.cpp qex.cpp
	$(CC) -o qex qex.cpp oGenData.cpp oAddData.cpp oMultData.cpp oManyToMany.cpp 


clean:
	rm ./*.o