#include<iostream>
#include"head.h"

bool running = false;
using namespace std;

void ProcList::unblock(){
  for(int i = 0; i < 4; i++){
    process *p = this->head;
    process *temp = nullptr;
    //find first in list blocked on device i
    while(p != nullptr){
      if(p->status == BLOCKED && p->device == i){
        break;
      }
      p = p->next;
    }
    /*if no processes where blocked on device i*/
    if(p == nullptr){
      continue;
    }else temp = p;
    /*see if any processes have been waiting for the same device for longer*/
    while(p != nullptr){
      if(p->status == BLOCKED && p->device == i && p->wait_tick < temp->wait_tick){
        temp = p;
      }
      p = p->next;
    } 
    /*double check we did everything right and unblock one process from this device if it has waited long enough*/
    if(temp->status == BLOCKED && temp->device == i && (TICK - temp->wait_tick) > 1024){
      temp->status = READY;
      cout << "Unblocking " << temp->name << " from device " << i << " and moving to READY after waiting for " << TICK-temp->wait_tick << " ticks" << endl;
    }
  }
}

void ProcList::banishProc() {
  process *p = this->head;
  process *prev = nullptr;

  while (p != nullptr) {
    if (p->status == EXITING) {
      if (prev != nullptr) {
        prev->next = p->next;
        delete p;
        p = prev->next;
      } else {
        // If the first node is EXITING, update the head
        this->head = p->next;
        delete p;
        p = this->head;
      }
      return;
    } else {
      prev = p;
      p = p->next;
    }
  }
  return;
}

    
void ProcList::io_event(int dev){
  cout << "io-event for device: " << dev << endl;
  process *p = this->head;
  while(p != nullptr){
    if(p->device == dev && p->status == BLOCKED){
      p->status = READY;
      p->device = -1;
      cout << "Process " << p->name << " is done waiting on device: " << dev << endl;
    }
    p = p->next;
  }  
}

void ProcList::query(string n){
  process *p = findProcess(n);
  p->print_proc();
}

void ProcList::query(){
  this->printAll();
}
  
void ProcList::release(){
  cout << "Moving current process from RUNNING to EXITING..." << endl;
  process *p = this->head;
  while(p != nullptr){
    if(p->status == RUNNING){
      p->status = EXITING;
      running = false;
      TICK += 32;
      return;
    }
    p = p->next;
  }
  cout << "No process currently RUNNING..." << endl;
}

void ProcList::step(){
   process *p, *temp;
  if(this->head == nullptr){
    cout << "No processes currently exist" << endl;
    TICK++;
    return;
  }
  /*handle the processes in exiting state first*/
  this->banishProc();
  /*move one process from NEW to READY*/
  p = this->head;
  while(p != nullptr){
    if(p->status == NEW){
      cout << "Advancing " << p->name << " from NEW to READY" << endl;
      p->status = READY;
      break;
    }
    p = p->next;
  }
  /*move one process from each IO device from
 *  BLOCKED to READY as long as it has waited 1024
 *  TICKS since it was blocked*/
  this->unblock(); 
  /*advance process from running state to ready state*/
  p = this->head;
  while(p != nullptr){
    if(p->status == RUNNING){
      cout << "Advancing " << p->name << " from RUNNING to READY..." << endl;
      p->status = READY;
      p->wait_tick = TICK;
      running = false;
      break;
    }
    p = p->next;
  }
  /*advance next process from ready state to running state*/
  if(!running){
    p = this->head;
    temp = nullptr;
    /*look fo the first ready process*/
    while(p != nullptr){
      if(p->status == READY){
        temp = p;
        break;
      }
      p = p->next;
    }
    /*look for the oldest*/
    while(p != nullptr){
      if((p->status == READY) && (p->wait_tick < temp->wait_tick)){
        temp = p;
      }
      p = p->next;
    }
    //if found, advance to running
    if(temp != nullptr){
      cout << "Advancing " << temp->name << " from READY to RUNNING" << endl;
      temp->status = RUNNING;
      running = true;
      TICK += 256;
      return;
    }else{
      //if not let the user know
      cout << "No process currently in READY state." << endl;
    }
  }
  TICK++;
}

process *ProcList::findProcess(string n){
  process *p = this->head;
  while(p!=nullptr){
    if(p->name == n){ return p; }
    p = p->next;  
  }
  cout << "PROCESS " << n << " DOES NOT EXIST" << endl;
  return nullptr;
}


void ProcList::block(int dev){
  if(!running){
    cout << "No process is currently running" << endl;
    return;
  }
  if((dev < 0) | (dev > 3)){
    cout << "You have not entered a valid device number." << endl;
    cout << "The only available devices are {0, 1, 2, 3}." << endl;
    return;
  }
  process *p = this->head;
  while(p != nullptr){
    if(p->status == RUNNING){
      cout << "Moving " << p->name << " from RUNNING to BLOCKED" << endl;
      p->status = BLOCKED;
      p->device = dev;
      p->wait_tick = TICK;
      running = false;
      TICK++;
      return;
    }
    p = p->next;
  }
  if(p == nullptr){
    cout << "ERROR in block(): No running process found" << endl;
  }
} 

void process::print_status(){
   switch(this->status){
    case -1:
      cout << "EXITING";
      break;
    case 0:
      cout << "NEW";
      break;
    case 1:
      cout << "READY";
      break;
    case 2:
      cout << "RUNNING";
      break;
    case 3:
      cout << "BLOCKED";
      break;
    default:
      cout << "STATUS ERROR";
      break;
  }
}

void process::print_proc(){
  cout <<"\n*************\nname: " << this->name;
  cout << "\nwait_tick: " << this->wait_tick << "  status: ";
  this->print_status();
  cout << endl;
  if(status == BLOCKED){
    cout << "This process is BLOCKED waiting on device: " << device;
    cout << "   since tick: " << this->wait_tick << endl;
  }
  cout << "\n*************\n";
  return;
}


void ProcList::addProcess(string name){
  process *p = new process(name);
  if(head != nullptr){
    process *ptr = head;
    while(ptr != nullptr){
      if(ptr->name == name){
        cout << "ERROR: Cannot create process with same name as existing process" << endl;
        return;
      }
      ptr = ptr->next;
    }
    this->tail->next = p;
    this->tail = p;
    this->tail->next = nullptr;
  }else{
    this->head = p;
    this->tail = p;
  }
  TICK += 32;
  return;
}

void ProcList::printAll(){
  if(running){
    cout << "There is currently a process running" << endl;
  }
  process *p = this->head;;
  while(p != nullptr){
    p->print_proc();
    p = p->next;
  }
  return;
}

