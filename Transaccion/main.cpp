#include <iostream>
#include <pthread.h> 
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include "../SequentialFile/SequentialFile.h"
#include "../SequentialFile/Record.h"
#define N_TO_READ 10


using namespace std;
pthread_mutex_t lockMutex = PTHREAD_MUTEX_INITIALIZER;

SequentialFile<Team<long> > teamsSequentialFile = SequentialFile<Team<long> >("../SequentialFile/data/Teams.bin", "../SequentialFile/data/TeamsSequentialFile.bin");
vector<Team<long> > responses;


void* fthread_write(void *t)
{
  pthread_mutex_lock(&lockMutex);
  teamsSequentialFile.insert(*((struct Team<long>*)t));
  pthread_mutex_unlock(&lockMutex);
  return NULL;
}

void* fthread_read(void *t)
{
  pthread_mutex_lock(&lockMutex);
  Team<long> temp = teamsSequentialFile.search(*((long*)t));
  pthread_mutex_unlock(&lockMutex);
  responses.push_back(temp);
  return NULL;
}

int main()
{

  pthread_t thread_write,thread_write2, thread_read[N_TO_READ];

  Team<long>* team1 = new Team<long>(99, "Jesus", 1, 1, 1, 1, 1, 1, 1, 1, 1);
  Team<long>* team2 = new Team<long>(100, "Pedro", 1, 1, 1, 1, 1, 1, 1, 1, 1);


  pthread_create(&thread_write, NULL, fthread_write, (void *)team1);
  pthread_create(&thread_write2, NULL, fthread_write, (void *)team2);

  pthread_join(thread_write,  NULL);
  pthread_join(thread_write2,  NULL);

///////

  long idToSearch[N_TO_READ] = {99L,838L,949L,308L,319L,404L,551L,515L,838L,956L};


  for (size_t i = 0; i < N_TO_READ; i++)
  {
    pthread_create(&thread_read[i], NULL, fthread_read, (void *)&idToSearch[i]);
  }

  for (size_t i = 0; i < N_TO_READ; i++)
  {
    pthread_join(thread_read[i],NULL);
  }
  
//   std::vector<Team<long> > teams = teamsSequentialFile.load();
//   for (Team<long> team : teams) {
//         Team<long> foundTeam = teamsSequentialFile.search(team.ID);
//         foundTeam.shortPrint();
//         std::cout << std::endl;
//   }

  for(auto& response : responses) {
      response.shortPrint();
      cout << "\n";
  }

  return 0;
}
