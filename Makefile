
all: main.cpp
	g++-4.8 main.cpp \
		-I/home/deividas/Desktop/storage/Soft/boost_1_55_0\
		-L/home/deividas/Desktop/storage/Soft/boost_1_55_0/stage/lib\
		-lboost_unit_test_framework

