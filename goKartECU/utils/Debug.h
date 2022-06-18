#ifndef DEBUG_H
#define DEBUG_H

class Debug {
private:
    bool enabled;
    void setup();
    void loop();
public:
    Debug(bool e) : enabled(e) {}
    void init() {
        if(!enabled) return;
        setup();
    }
    void run() {
        if(!enabled) return;
        while(true) {
          loop();
        }
    }
};


void Debug::setup() {
    // put your setup code here, to run once:
}

void Debug::loop() {
    // put your main code here, to run repeatedly:
}

#endif
