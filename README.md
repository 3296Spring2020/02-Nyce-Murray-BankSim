# BankSim

### The Initial Race Condition

##### Explanation

Originally, `Bank_open()` starts a loop, creating multiple threads that all interact asynchronously with the list of accounts, moving money from one to another. There is a race condition here, where interrupts occur while threads access the accounts, allowing control to pass to other threads without account balances being updated. For instance, thread A could be interrupted right after it checks to see if an account has sufficient funds. Thread B could then also accesses that account and move money to a different account. Control could then be returned to thread A, which then withdraws money from that account that is no longer there. If thread A then deposits that money in another account, we have just created money that doesn't exist.

##### Program Flow

![](./race-condition.png)

JM- 1) This project required us to collaborate in building a multithreaded bank simulator in C. We were given source code for the simulator, but it was missing synchronization protection. We had to first identify the critical sections of the code, and add code to protect them. For the first task, Phil created a UML sequence diagram to visualize the flow of data through the simulation. In task 2, I added locks for each account that are obtained whenever the withdraw() or deposit() functions are called. Task 3 required us to protect another race condition in which the testing of the bank is interrupted, causing the test to fail. Phil created a mutex lock for the bank itself. For task 4, I added condition variables so that when an account does not have sufficient funds to make a transfer, it waits until its balance is high enough to withdraw the desired amount of money. Task 5 eliminates of the possibility of a deadlock occuring if one thread exits before the others. This was taken care of by handing the bank lock to the Bank_test thread as soon as the first thread exits its loop. 
JM- 2) The coding process of this assignment was collaborative the whole way through, especially in the second half. One of us would come up with a solution, and then the other would test it to see if it worked. We got especially stuck on tasks 3 and 4, so we each went back and forth changing the code for both of those parts, as well as sitting and figuring it out together. 
JM- 3) The majority of the testing for this program was done by using printf statements, as C does not include unit testing support, or proper debugging for multithreaded programs. Some of the bugs were discovered just by studying the output of the program, but most of them were indentified with print statements. The tests were mostly written after the code for this project. However, we did write tests for each other's tasks. 

