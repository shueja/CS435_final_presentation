#include "mbed.h"
#include "NumberDisplay.h"

NumberDisplay num_disp(D13, D14, D15);
DigitalOut elev_up(D7);
DigitalOut elev_down(D4);

DigitalIn light_sensor(D8);
struct cache_t {
    bool panel[4];
    bool car[4];
    bool light;
};

struct cache_t cache = {
    .panel = {false},
    .car = {false},
    .light = false
};
bool prev_light_sensor = light_sensor.read();
Timer holdTime;
// InterruptIn car_floor0(D0);
// InterruptIn car_floor1(D1);
// InterruptIn car_floor2(D2);
// InterruptIn car_floor3(D3);
DigitalIn car_button[4] {NC, NC, D2, D3};
DigitalIn panel_button[4] {D9, D10, D11, D12};
PwmOut buzzer(D6);
#define MAX_FLOOR 4
int floor_above = 0;
int floor_below = 0;

bool requests[4] = {false};
int setpoint = 0;
int previousDirection = 0;
int command = 0;
bool estop = false;
bool anyRequestsAbove() {
    for (int i = floor_above; i < MAX_FLOOR; i++) {
        if (requests[i]) {
            return 1;
        }
    }
    return 0;
}
bool anyRequestsBelow() {
    for (int i = floor_below; i >= 0; i--) {
        if (requests[i]) {
            return 1;
        }
    }
    return 0;
}
int nextDirection() {
    if (floor_above != floor_below) {
        return previousDirection;
    }
    //floor_above == floor_below
    if (requests[floor_below]) {
        holdTime.reset();
        holdTime.start();
        requests[floor_below] = false;
        return 0;
    }
    if (holdTime.elapsed_time() < 2s) {
        return 0;
    } else {
        holdTime.stop();
    }

    if (previousDirection >= 0) {
        if (anyRequestsAbove()) {
            return 1;
        } else if (anyRequestsBelow()) {
            return -1;
        } else {
            // up to next floor
            return floor_above - floor_below;
        }
    }
    if (previousDirection < 0) {
        if (anyRequestsBelow()) {
            return -1;
        } else if (anyRequestsAbove()) {
            return 1;
        } else {
            // Down to next floor
            return floor_below - floor_above;
        }
    }
    return 0;
}

void reachGap() {
   
    if (command > 0 && floor_above <= MAX_FLOOR) {
        previousDirection = 1;
        floor_below = floor_above;
    }
    else if (command < 0 && floor_below >= 0) {
        previousDirection = -1;
        floor_above = floor_below;
    }
}

void leaveGap() {
    if (command > 0) {floor_above = floor_below + 1;}
    else if (command < 0) {floor_below = floor_above - 1;}
}

void setpoint_to_command () {
    if (floor_above == floor_below && floor_below == setpoint) {
        command = 0;
    } else if (floor_below < setpoint) {
        command = 1;
    } else if (floor_above > setpoint) {
        command = -1;
    }
}
void run_elev () {
    elev_up = command > 0 && !estop;
    elev_down = command < 0 && !estop;
}
// main() runs in its own thread in the OS
int main()
{
    for (int i = 0; i < MAX_FLOOR; i++) {
        panel_button[i].mode(PullUp);
        car_button[i].mode(PullUp);
    }

    light_sensor.mode(PullUp);
    holdTime.start();
    buzzer.period_ms(20.0);
    // panel_floor0.rise(&request0);
    // panel_floor1.rise(&request1);
    // panel_floor2.rise(&request2);
    // panel_floor3.rise(&request3);
    printf("Elevator\n");
    while (true) {

        // Update cache
        cache.light = light_sensor.read();
        for (int i = 0; i < MAX_FLOOR ; i++) {
            cache.panel[i] = !panel_button[i].read();
            cache.car[i] = !car_button[i].read();
            if (cache.panel[i]) {
                requests[i] = true;
            }
        }
        // Log
        printf("Above: %d Below: %d, Set: %d Com: %d PLS: %d LS: %d Req: %d %d %d %d\r\n", floor_above, floor_below, setpoint, command,
      
        (int) prev_light_sensor,
        (int) cache.light,
        (int) requests[0],
        (int) requests[1],
        (int) requests[2],
        (int) requests[3]);

        
        // gap tracking
        if (!prev_light_sensor && cache.light) {
            reachGap();
        } else if (prev_light_sensor && !cache.light) {
            leaveGap();
        }
        prev_light_sensor = cache.light;

        // Command logic
        // if (cache.panel[0]) {setpoint = 0;}
        // else if (cache.panel[1]) {setpoint = 1;}
        // else if (cache.panel[2]) {setpoint = 2;}
        // else if (cache.panel[3]) {setpoint = 3;}
        command = nextDirection();
        previousDirection = (command != 0) ? command : previousDirection;

        uint8_t numToShow = previousDirection > 0 ? floor_below : floor_above;
        if      (cache.car[2]) {command = -1;}
        else if (cache.car[3]) {command = 1;}

        run_elev();
        
        num_disp.Point(holdTime.elapsed_time() < 2s);
        num_disp.Show(Number(numToShow + 1));
        auto time = holdTime.elapsed_time();
        if (time > 500ms && time < 1s) {
            buzzer.period(0.0025);
            buzzer.write(0.75);
        } else
        if (time > 1100ms && time < 1900ms) {
            buzzer.period(0.003);
            buzzer.write(0.75);
        } else {
            buzzer.write(0.0);
        }
        
        ThisThread::sleep_for(40ms);
    }

    return 0;
}

