#ifndef COMMON_H
#define COMMON_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Account {
    int balance;
    int id;
    int fundsneeded;
    pthread_t thread;
    pthread_mutex_t accountlock;
    pthread_cond_t lowfunds;
} Account;

typedef struct Bank {
    int initialBalance;
    int numAccounts;
    long ntransacts;
    int ntransactsInProgress;
    Account **accounts;
    pthread_mutex_t bankLock;
} Bank;

typedef struct TransferThreadParameters {
    Bank *b;
    int fromAccount;
    int maxAmount;
} TransferThreadParameters;

#endif