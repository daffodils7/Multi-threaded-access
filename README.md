# Multi-threaded-access  

This is used to solve the written test problem.  
The relevant code is located in the main.cpp file and can be run according to the output prompt:  
Enter 'x y' to specify the range of element sizes for array S, with a default value of [0,100]. Enter enter to select the default value.  
Enter M to specify the number of threads (employees) to operate simultaneously.  
Enter Y/N to specify whether to monitor the process of updating the array, and the order of the output process will be disrupted due to thread parallelism.  
  
Will deadlocks occur?  
ANS:NO!  
There is unlikely to be a deadlock because we are using std::shared_mutex and std::unique_lock. std::shared_mutex allows multiple threads to access it concurrently, and std::unique_lock is a movable exclusive lock that can delay locking in the constructor, thus avoiding deadlock.  
  
How to avoid deadlocks?  
1.Avoiding multiple threads attempting to acquire multiple locks simultaneously.  
2.Keeping the lock acquisition order consistent. For example, if one thread acquires lock A first and then lock B, other threads should acquire locks A and B in the same order.  
3.Using smart locks or lock combinations, such as std::lock_guard or std::scoped_lock, which automatically release locks when their scope ends, avoiding the possibility of forgetting to unlock.  
4.Designing thread communication and synchronization mechanisms carefully to minimize the use of locks.  
