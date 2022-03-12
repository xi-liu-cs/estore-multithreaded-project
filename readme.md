# estore multithreaded project
Xi Liu </br>
2021 </br>
* an online store that allows concurrent updates to the inventory database, modification requests are from suppliers and customers that are generating threads
* uses both coarse-grained and fine grained store synchronizations
to maintain the internal integrity of data structures 
* uses facilities provided by the POSIX threads interface pthreads.h
* in EStore.cpp, various utilities such as addItem(), buyManyItems(), ..., are critical sections controlled by conditional variables and locks in both coarse-grained and fine grained mode
* in taskQueue.h, a queue is managed for current waiting tasks, each task is represented
by a struct Task, which contain a function pointer which will be executed when the task is being dequeued