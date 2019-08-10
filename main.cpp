#include "Xbox.h"
#include "SpeedController.h"
#include "Exceptions.h"
#include "Utils.h"

#include <signal.h>
#include <stdio.h>
#include <stdint.h>

#define UPDATE_RATE 50 //hz

#define CURVE 2.5
#define DEADZONE 0.1

#define SPEED 1

class Robot {
  private:
    Xbox *xbox;
    SpeedController *frontLeft;
    SpeedController *frontRight;
    SpeedController *backLeft;
    SpeedController *backRight;
  public:
    VMXPi *vmx;
    
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
      printf("Setting all motors to zero!\n");
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
      xbox->pollEvents();

      if(xbox->isConnected()) {
        double m_r = xbox->getX(RIGHT);
        double t_r = xbox->getY(LEFT);

        m_r = deadzone(m_r, DEADZONE);
        t_r = deadzone(t_r, DEADZONE);

        m_r = curve(m_r, CURVE);
        t_r = curve(t_r, CURVE);

        this->arcadeDrive(m_r, t_r, SPEED);
      } else {
        this->stop();
      }
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

void handle_signal(int signo) {
  if(signo == SIGINT)
    interrupted = true;
}

void run_timer(void *param, uint64_t timestamp_us) {
  void **param_list = (void **)param;
  Robot *robot = (Robot *)param_list[0];

  robot->mainLoop();
}

int main(int argc, char *argv[]) {
  Robot *robot = NULL;

  try {
    robot = new Robot;

    struct sigaction sa;
    
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigset_t int_mask, open_mask;
    sigemptyset(&int_mask);
    sigemptyset(&open_mask);
    sigaddset(&int_mask, SIGINT);

    sigprocmask(SIG_SETMASK, &int_mask, NULL);

    sigaction(SIGINT, &sa, NULL);

    void *param_list[2];
    param_list[0] = robot;
    param_list[1] = NULL;

    robot->vmx->time.RegisterTimerNotificationRelative(run_timer, 1000000/UPDATE_RATE, (void *)param_list, true);

    while(!interrupted) {
      sigsuspend(&open_mask);
    }

    delete robot;
  } catch (int e) {
    printf("Exception 0x%02x\n", e);
    if(robot) {
      robot->stop();
    }
  }
}