#ifndef HEAD_H
#define HEAD_H

#include<string>
using namespace std;

#define NEW 0
#define READY 1
#define RUNNING 2
#define BLOCKED 3
#define EXITING -1

extern int TICK;
extern bool running;
class process{
 public:
  string name;
  int wait_tick, status, device;
  process *next;
  process(string n){ name = n; next = nullptr; 
                     wait_tick = TICK; status = NEW; 
                     device = -1;}
  
  void print_proc();
  void print_status();
  void set_status(int s){status = s;}
  
};

class ProcList{
  public:
  process *head, *tail;
  

  ProcList(){
    head = nullptr;
    tail = nullptr;
  }
  
  ~ProcList(){
    process *current = head;
    process *temp = current;
    while (current != nullptr) {
       temp = current;
       current = current->next;
       delete temp;
    }
  }
  process *findProcess(string n);
  
  void addProcess(string name);
  void ready(string n);
  void block(int dev);
  void unblock();
  void step();
  void query(string);
  void query();
  void release();
  void io_event(int);
  void banishProc();
  void printAll();
  
 
};

string *getcom();
#endif
