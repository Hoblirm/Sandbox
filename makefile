make:
	rm -rf boost_proc
	rm -rf mutex_proc
	rm -rf sem_proc
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DBOOSTPROC -o boost_proc main.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DMUTEXPROC -o mutex_proc main.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
	g++ -O2 -DBOOST_DATE_TIME_NO_LIB -DSEMPROC -o sem_proc main.cpp -I./inc -I/home/hobby/dev/boost_1_60_0 -lpthread -lrt
