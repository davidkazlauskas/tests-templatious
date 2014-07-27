
all: main.cpp
	g++-4.8 main.cpp \
		-std=c++11\
		-O0\
		-g\
		-I/home/deividas/Desktop/storage/Soft/boost_1_55_0\
		-L/home/deividas/Desktop/storage/Soft/boost_1_55_0/stage/lib\
		-I/home/deividas/Desktop/Dropbox/MyPrj/Templatious\
		-lboost_unit_test_framework

