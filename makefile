CC=g++

qexmake: oGenData.cpp  oppManyToMany.cpp oppManyToSome.cpp qex.cpp
	$(CC) -o qex qex.cpp oGenData.cpp oppManyToMany.cpp oppManyToSome.cpp 


clean:
	rm ./*.o