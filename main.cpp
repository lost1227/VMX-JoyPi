#include "Robot.h"

#include <signal.h>
#include <stdio.h>
#include <stdint.h>


bool interrupted = false;

void handle_signal(int signo) {
  if(signo == SIGINT || signo == SIGTERM)
    interrupted = true;
}

void wait_for_sigint() {
  struct sigaction sa;

  sa.sa_handler = handle_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  
  sigset_t int_mask, open_mask;
  sigemptyset(&int_mask);
  sigemptyset(&open_mask);
  sigaddset(&int_mask, SIGINT);
  sigaddset(&int_mask, SIGTERM);

  sigprocmask(SIG_SETMASK, &int_mask, NULL);

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);

  printf("Waiting for sigint...\n");
  while(!interrupted) {
    sigsuspend(&open_mask);
  }

  printf("SIGINT received!");
}


int main() {
  Robot *robot = NULL;

  try {
    robot = new Robot;
    robot->start();
    wait_for_sigint();
    delete robot;
  } catch (int e) {
    printf("Exception 0x%02x\n", e);
    if(robot)
      delete robot;
  }
}
