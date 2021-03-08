//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "FloodingRSU.h"

Define_Module(FloodingRSU);
/**
 *
 *
 *
 * Do not change this code!
 *
 *
 *
 *
 */
void FloodingRSU::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        int randomDelay = intuniform(0, 2);
        startFlooding = new FloodingMessage();
        startFlooding->setKind(1234);
        scheduleAt( this->getSimulation()->getWarmupPeriod() + 3 + randomDelay, startFlooding);
    }
}

void FloodingRSU::handleMessage(cMessage *msg) {
    if(msg == startFlooding){
            FloodingMessage* fm = new FloodingMessage();
            Coord originPos;
            originPos.x = 1450;
            originPos.y = 1250;
            fm->setOriginPosition(originPos);
            simtime_t delay = uniform(0.0, 1.0);
            sendDelayedDown(fm, delay);
            getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"green");
    }
    delete msg;
}
FloodingRSU::FloodingRSU(){

}
FloodingRSU::~FloodingRSU() {
    sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);
    sendWSAEvt = new cMessage("wsa evt", SEND_WSA_EVT);
}
