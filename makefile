make:
	rm -rf proc1
	rm -rf proc2
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -o proc1 main.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -o proc2 main2.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
