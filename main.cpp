#include <iostream>

#include <sys/time.h>
#include <signal.h>
#include <stdio.h>

#include "Joy.h"

bool interrupted = false;
bool alarm = false;

void handle_interrupt(int signo) {
   interrupted = true;
}

void handle_alarm(int signo) {
  alarm = true;
}

void print_joy(Joy *joy) {
  int i;
  int num_axes = joy->getNumAxes();
  int num_buttons = joy->getNumButtons();

  for(i = 0; i < num_axes; i++) {
    printf("axis%d:%.4f ",i, joy->getAxisState(i));
  }
  for(i = 0; i < num_buttons; i++) {
    printf("button%d:%d ",i,(joy->getButtonState(i))?1:0);
  }
  printf("\n\n");
}

int main() {
  try {
  Joy joy(0);
  struct sigaction sa_int, sa_alrm;
  
  sa_int.sa_handler = handle_interrupt;
  sigemptyset(&sa_int.sa_mask);
  sa_int.sa_flags = 0;
  
  sa_alrm.sa_handler = handle_alarm;
  sigemptyset(&sa_alrm.sa_mask);
  sigaddset(&sa_alrm.sa_mask, SIGALRM);
  sa_alrm.sa_flags = 0;
  
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 20;
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 20;

  sigset_t alarm_mask, open_mask;
  sigemptyset(&alarm_mask);
  sigemptyset(&open_mask);
  sigaddset(&alarm_mask, SIGINT);
  sigaddset(&alarm_mask, SIGALRM);

  sigprocmask(SIG_SETMASK, &alarm_mask, NULL);

  sigaction(SIGINT, &sa_int, NULL);
  sigaction(SIGALRM, &sa_alrm, NULL);
  setitimer(ITIMER_REAL, &timer, NULL);

  while(!interrupted) {
    if(alarm) {
      alarm = false;
      joy.pollEvents();
      print_joy(&joy);
    }
    sigsuspend(&open_mask);
  }
  sigprocmask(SIG_SETMASK, &open_mask, NULL);
  } catch (int e) {
    printf("Error %d\n", e);
    return e;
  }
  return 0;
}
