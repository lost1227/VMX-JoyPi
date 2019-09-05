#include "Robot.h"
#include "Utils.h"

#define CURVE 2.5
#define DEADZONE 0.1

#define SPEED 1

Robot::Robot() : TimedRobot() {
  xbox = new Xbox(0);
  drive = new DifferentialDrive(vmx, 0, 1);
  drive->setReversed(true);

  horn = new SpikeRelay(vmx, 2, 3);
}

Robot::~Robot() {
  delete xbox;
  delete drive;
  delete horn;
}

void Robot::robotPeriodic() {
  xbox->pollEvents();

  if(xbox->isConnected()) {
    double m_r = xbox->getY(LEFT);
    double t_r = xbox->getX(RIGHT);

    m_r = Utils::deadzone(m_r, DEADZONE);
    t_r = Utils::deadzone(t_r, DEADZONE);

    m_r = Utils::curve(m_r, CURVE);
    t_r = Utils::curve(t_r, CURVE);

    //printf("m_r:%.2f t_r:%.2f\n", m_r, t_r);

    drive->arcadeDrive(m_r, t_r, SPEED);

    bool honk = xbox->getA();

    if(honk) {
      horn->set(SpikeRelay::forward);
    } else {
      horn->set(SpikeRelay::off);
    }

  } else {
    printf("Xbox controller disconnected\n");
    drive->stop();
  }
}
