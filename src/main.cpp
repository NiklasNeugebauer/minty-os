#include "MintyBase.h"
#include "WatchFaces/HelloWorld.h"
#include "WatchFaces/BasicTime.h"
#include "WatchFaces/PebbleClock.h"

//#include "./apps/_experiments/hello_world.h"

MintyBase base;

//HelloWorld watch_face;
//BasicTime watch_face;
PebbleClock watch_face;

void setup () {
    base.wakeupRoutine();
    watch_face.draw(base.display);
    base.deepSleep();
}

void loop () {

}
