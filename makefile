CC=g++

qexmake: oGenData.cpp oAddData.cpp oMultData.cpp oppManyToMany.cpp oppManyToSome.cpp qex.cpp
	$(CC) -o qex qex.cpp oGenData.cpp oAddData.cpp oMultData.cpp oppManyToMany.cpp oppManyToSome.cpp 


clean:
	rm ./*.o