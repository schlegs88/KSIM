#include<iostream>
#include"head.h"
#include<iomanip>
#define MAX_COMMAND 256
using namespace std;
int TICK = 0;
int main(){
  bool exit = 0;
  ProcList *p = new ProcList;
  string *commands = new string[MAX_COMMAND];
  while(!exit){
    cout << "ksim-" << setw(9) << setfill('0') << TICK << ">";
    // Use a temporary array to avoid memory leak
    string *newCommands = getcom();
    // Copy the contents from the new array to the existing one
    copy(newCommands, newCommands + MAX_COMMAND, commands);
    delete[] newCommands;

    if(commands[0] == "exit"){
      cout << "Now exiting..." << endl;
      delete[] commands;
      delete p;
      return 0;
    }else if(commands[0] == "add"){
      p->addProcess(commands[1]);
    }else if(commands[0] == "step"){
      p->step();
    }else if(commands[0] == "wait"){
      int s = stoi(commands[1]);
      p->block(s);
    }else if(commands[0] == "release"){
      p->release();
    }else if(commands[0] == "query") {
      if(commands[1] == "") { p->query(); }else{ p->query(commands[1]); }
    }else if(commands[0] == "io-event"){
      int s = stoi(commands[1]);
      p->io_event(s);
    }else{
        cout << "YOU HAVE NOT ENTERED A VALID COMMAND" << endl;
    }
    /*This is where I need to put my parser from assignment 3*/
    /*could change it up and whip out a strtok() tho*/
  }  
  return 0;
}

