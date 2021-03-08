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

#include "FloodingBase.h"

Define_Module(FloodingBase);

void FloodingBase::initialize(int stage) {
    DemoBaseApplLayer::initialize(stage);
    if (stage == 0) {
        //Initializing members and pointers of your application goes here

        duplicates.setName("duplicates");
        sentMessages.setName("sent_messages");
        reachedNodes.setName("reached_nodes");
        freshMessage.setName("freshMessage");
        received.setName("received");

        getParentModule()->getDisplayString().setTagArg("i", 1, "red");
        sig_hopcount = registerSignal("sig_hopcount");
        sig_delay = registerSignal("sig_delay");
//        numDuplicates = 0;
//        numSent = 0;
//
//        newFirstMessage = false;
//        isDuplicate = false;
//        numfreshMessage = 0;

    } else if (stage == 1) {
        //Initializing members that require initialized other modules goes here
        // globalStats = FindModule<GlobalStatistics*>::findSubModule(getParentModule()->getParentModule());
    }
}

void FloodingBase::handleMessage(cMessage *msg) {
    if (FloodingMessage *fm = dynamic_cast<FloodingMessage*>(msg)) {
        handleFloodingMsg(fm);
    } else if (msg->isSelfMessage()) {
        handleSelfMsg(msg);
    }
}
void FloodingBase::handleFloodingMsg(FloodingMessage *msg) {
//    bool isDuplicate = false;
//    getParentModule()->getDisplayString().setTagArg("i", 1, "green");
//    annotations->scheduleErase(3,
//            annotations->drawLine(curPosition, msg->getOriginPosition(),
//                    "red"));
//    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1,
//            "yellow");
//
//    //msg->setHops(msg->getHops() + 1);
//
//    traciVehicle->setColor(veins::TraCIColor(255, 0, 0, 255));
//    if (isDuplicate) { //TODO: Check for duplicates here
//        numDuplicates++;
//        duplicates.record(1);
//    } else {
//        findHost()->getDisplayString().updateWith("g=16,green"); // If a car received the message, the car will appear with green color in the UI
//    }
//    getParentModule()->getDisplayString().setTagArg("i2", 0, "status/busy");
}
void FloodingBase::finish() {
    DemoBaseApplLayer::finish();
    //maybe you want to record some scalars?

    //recordScalar("#overallReachedNodes", overallReachedNodes);
//    recordScalar("#numfreshMessage", numfreshMessage);
//    recordScalar("#duplicates", numDuplicates);
//    recordScalar("#numSent", numSent);

}

void FloodingBase::handleSelfMsg(cMessage *msg) {
    DemoBaseApplLayer::handleSelfMsg(msg);
    //this method is for self messages (mostly timers)
    //it is important to call the DemoBaseApplLayer function for BSM and WSM transmission

}
