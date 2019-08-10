#include <iostream>

#include <sys/time.h>
#include <signal.h>
#include <stdio.h>

#include "Xbox.h"

bool interrupted = false;
bool alarm = false;

void handle_interrupt(int signo) {
   interrupted = true;
}

void handle_alarm(int signo) {
  alarm = true;
}

void print_xbox(Xbox *xbox) {
  if(xbox->isConnected()) {
    printf("L_Joy(x:%.4f,y:%.4f,b:%d)", xbox->getX(LEFT), xbox->getY(LEFT), xbox->getStickPressed(LEFT));
    printf(" R_Joy(x:%.4f,y:%.4f,b:%d)", xbox->getX(RIGHT), xbox->getY(RIGHT), xbox->getStickPressed(RIGHT));
    printf(" Trigger(L:%.4f,R:%.4f)", xbox->getTrigger(LEFT), xbox->getTrigger(RIGHT));
    printf(" Bumper(L:%d,R:%d)", xbox->getBumper(LEFT), xbox->getBumper(RIGHT));
    printf(" POV(%d)", xbox->getPOV());
    printf(" (A:%d,B:%d,X:%d,Y:%d)", xbox->getA(), xbox->getB(), xbox->getX(), xbox->getY());
    printf(" (B:%d,CB:%d,S:%d)", xbox->getBack(), xbox->getCenterButton(), xbox->getStart());
    printf("\n");
  } else {
    printf("Xbox not connected!\n");
  }
}

int main() {
  try {
  Xbox xbox(0);
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
      xbox.pollEvents();
      print_xbox(&xbox);
    }
    sigsuspend(&open_mask);
  }
  sigprocmask(SIG_SETMASK, &open_mask, NULL);
  } catch (int e) {
    printf("Error 0x%02x\n", e);
    return e;
  }
  return 0;
}
