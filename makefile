make:
	rm -rf boost_proc1
	rm -rf boost_proc2
	rm -rf mutex_proc1
	rm -rf mutex_proc2
	rm -rf sem_proc1
	rm -rf sem_proc2
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DBOOSTPROC -o boost_proc1 main.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DBOOSTPROC -o boost_proc2 main2.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DMUTEXPROC -o mutex_proc1 main.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DMUTEXPROC -o mutex_proc2 main2.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DSEMPROC -o sem_proc1 main.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DSEMPROC -o sem_proc2 main2.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
