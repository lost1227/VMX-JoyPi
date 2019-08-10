#include "Xbox.h"
#include "SpeedController.h"
#include "Exceptions.h"
#include "Utils.h"

#include <sys/time.h>
#include <signal.h>

#define UPDATE_RATE 50 //hz

#define CURVE 2.5
#define DEADZONE 0.1

#define SPEED 1

class Robot {
  private:
    VMXPi *vmx;
    Xbox *xbox;
    SpeedController *frontLeft;
    SpeedController *frontRight;
    SpeedController *backLeft;
    SpeedController *backRight;
  public:
    Robot() {
      vmx = new VMXPi(false, UPDATE_RATE);

      if(!vmx->IsOpen()) {
        throw E_VMX_NOT_OPEN;
      }

      xbox = new Xbox(0);
      frontLeft = new SpeedController(vmx, 0);
      frontRight = new SpeedController(vmx, 1);
      backLeft = new SpeedController(vmx, 2);
      backRight = new SpeedController(vmx, 3);

    }

    void stop() {
      backRight->setSpeed(0);
      backLeft->setSpeed(0);
      frontRight->setSpeed(0);
      frontLeft->setSpeed(0);
    }

    void arcadeDrive(double m_r, double t_r, double speed_limit) {
      m_r = clip(m_r, -1, 1);
      t_r = clip(t_r, -1, 1);
      speed_limit = clip(speed_limit, 0, 1);
      double left_side = (m_r + t_r) * speed_limit;
      double right_side = (m_r - t_r) * speed_limit;

      frontLeft->setSpeed(left_side);
      backLeft->setSpeed(left_side);
      frontRight->setSpeed(right_side);
      backRight->setSpeed(right_side);
    }

    void mainLoop() {
      double m_r = xbox->getX(RIGHT);
      double t_r = xbox->getY(LEFT);

      m_r = deadzone(m_r, DEADZONE);
      t_r = deadzone(t_r, DEADZONE);

      m_r = curve(m_r, CURVE);
      t_r = curve(t_r, CURVE);

      this->arcadeDrive(m_r, t_r, SPEED);
    }

    ~Robot() {
      this->stop();

      delete backRight;
      delete backLeft;
      delete frontRight;
      delete frontLeft;
    }
};

bool interrupted = false;
bool alarm = false;

void handle_signal(int signo) {
  if(signo == SIGINT)
    interrupted = true;
  else if(signo == SIGALRM)
    alarm = true;
}

int main(int argc, char *argv[]) {

  Robot robot;

  struct sigaction sa;
  
  sa.sa_handler = handle_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  
  struct itimerval timer;
  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec = 1000/UPDATE_RATE;
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 1000/UPDATE_RATE;

  sigset_t alarm_mask, open_mask;
  sigemptyset(&alarm_mask);
  sigemptyset(&open_mask);
  sigaddset(&alarm_mask, SIGINT);
  sigaddset(&alarm_mask, SIGALRM);

  sigprocmask(SIG_SETMASK, &alarm_mask, NULL);

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGALRM, &sa, NULL);
  setitimer(ITIMER_REAL, &timer, NULL);

  while(!interrupted) {
    if(alarm) {
      alarm = false;
      robot.mainLoop();
    }
    sigsuspend(&open_mask);
  }
}