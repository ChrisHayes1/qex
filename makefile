CC=g++

qexmake: oGenData.cpp oGenDataFull.cpp  oppManyToMany.cpp oppManyToSome.cpp qex.cpp
	$(CC) -o qex qex.cpp oGenData.cpp oGenDataFull.cpp oppManyToMany.cpp oppManyToSome.cpp 


gen: gen_data.cpp
	$(CC) -o gen_data gen_data.cpp

read: read_data.cpp
	$(CC) -o read_data read_data.cpp


clean:
	find . -type f -executable -exec rm '{}' \;