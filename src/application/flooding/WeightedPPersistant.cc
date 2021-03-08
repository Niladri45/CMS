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

#include "WeightedPPersistant.h"
#include <algorithm>
#include <cmath>
#include <vector>

Define_Module(WeightedPPersistant);

int WeightedPPersistant::overallReachedNodes;

double nodejx;
double nodejy;
double nodejz;
double nodeix;
double nodeiy;
double nodeiz;
long double interferenceRangeR = 100, pij = 0, pijMin = 0, dist = 0;
double cutOff = 0.4, cutOffNew = 0.125;
double wait_time = 8;
double delay = 2;
int id;
std::set<int> msgIdList;
std::set<int> dupList;

long numSent = 0;
long numDuplicates = 0;
bool newFirstMessage = false;
bool isDuplicate = false;
long numfreshMessage = 0;

unsigned int got1stTime = 0;

void WeightedPPersistant::initialize(int stage) {
    FloodingBase::initialize(stage);
    if (stage == 0) {
        // Initializing members and pointers of your application goes here
//        overallReachedNodes = 99;
//        newFirstMessage = false;
//        isDuplicate = false;
//        numfreshMessage = 99;

//        numSent = 0;
//        numDuplicates = 0;
//        newFirstMessage = false;
//        isDuplicate = false;
//        numfreshMessage = 0;

        curPosition = mobility->getPositionAt(simTime()); // set current position during initialization

    } else if (stage == 1) {
        // Initializing members that require initialized other modules goes here
    }
}

void WeightedPPersistant::finish() {
    // FloodingBase::finish();
    // maybe you want to record some scalars?
    recordScalar("#overallReachedNodes", overallReachedNodes);
    recordScalar("#newReachedNodes", numfreshMessage);
    recordScalar("#duplicates", numDuplicates);
    recordScalar("#numSent", numSent);

    int pid = getParentModule()->getIndex();
    std::cout << "MAC ID: " << pid << " Number sent: " << numSent << '\n';

}

void WeightedPPersistant::handleFloodingMsg(FloodingMessage *msg) {
    FloodingBase::handleFloodingMsg(msg);

    overallReachedNodes++;
    reachedNodes.record(overallReachedNodes);

    id = getParentModule()->getIndex();
    std::cout << "MAC: " << id << endl;

    received.record(id);

    simtime_t NewReceptionTime = simTime(); // get simTime() when message is received
    // simtime_t OldReceptionTime = NewReceptionTime; // get simTime() when
    // message is received

    isDuplicate = false;
    getParentModule()->getDisplayString().setTagArg("i", 1, "green");
    annotations->scheduleErase(3,
            annotations->drawLine(curPosition, msg->getOriginPosition(),
                    "red"));
    getParentModule()->getParentModule()->getDisplayString().setTagArg("i", 1,
            "yellow");
    traciVehicle->setColor(veins::TraCIColor(255, 0, 0, 255));

    /*
     *   To find if same sender sent the message again.
     */
    if ((!msgIdList.empty() && msgIdList.find(id) == msgIdList.end())
            || msgIdList.empty()) { // check for duplicate MAC ID
        msgIdList.insert(id);                      // new ID
        newFirstMessage = true;

        numfreshMessage++;

        freshMessage.record(id);

    } else {
        // already exists- duplicate message
        isDuplicate = true;

        //msg->setHops(msg->getHops() + 1);

        numDuplicates++;
        duplicates.record(id);

        findHost()->getDisplayString().updateWith("g=16,green"); // If a car received the message, the car will appear with green color in the UI

        getParentModule()->getDisplayString().setTagArg("i2", 0, "status/busy");

        newFirstMessage = false;
        dupList.insert(id); // insert MAC ID, if message received for 2nd time
    }

    // Case 1: Message received for the 1st time, send
    // without any further delay
    if (newFirstMessage) {

        // Just for checking - print message
        std::cout << "MAC ID List: " << endl;

        for (std::set<int>::iterator s = msgIdList.begin();
                s != msgIdList.end(); s++) {
            std::cout << *s << "  ";
        }
        std::cout << endl;

        Coord *senderCoord = new Coord(); // For sender
        senderCoord = &(msg->FloodingMessage::getOriginPosition());

        nodeix = senderCoord->x;
        nodeiy = senderCoord->y;
        nodeiz = senderCoord->z;

        //std::cout << "Sender position: " << nodeix << " " << nodeiy << " "
        // << nodeiz << " " << endl;

        curPosition = mobility->getPositionAt(simTime()); // For Receiver
        msg->setOriginPosition(curPosition);
        nodejx = curPosition.x;
        nodejy = curPosition.y;
        nodejz = curPosition.z;

        //std::cout << "Receiver position: " << nodejx << " " << nodejy << " "
        //<< nodejz << " " << endl;

        // 1. Distance:
        dist = sqrt(
                pow(nodeix - nodejx, 2) + pow(nodeiy - nodejy, 2)
                        + pow(nodeiz - nodejz, 2));
        pij = dist / interferenceRangeR;
        pijMin = pij; // initialize

        //std::cout << "Distance: " << dist;
//        std::cout << " 1st case Pij: " << pij << endl;

        // TODO: The following code block might help you - the statistics are wrong
        // at the moment

        msg->setOriginPosition(curPosition);
        //simtime_t delay = uniform(0, 0.1); // delay function
        if (pij >= cutOff) { // pij >= cutOff
            sendDelayedDown(msg->dup(), delay);
            std::cout << "SENT 1st case Pij: " << pij << endl;
            sentMessages.record(id);
            numSent++;
        } else {
            std::cout << "NOT sent 1st case Pij: " << pij << endl;
            // don't send
        }
    }      // case 1 over
    else { // case 2 begins when duplicate message received
           //std::cout << "Inside 2nd case" << endl;
        bool dupReceived = false;

        if (simTime() > NewReceptionTime + wait_time) {
            //std::cout << "2nd case: duplicate message send direct" << endl;
            sendDelayedDown(msg->dup(), delay);
            numSent++;
        } else { // send message with Pij
            //std::cout << "2nd case: duplicate message send with minimum pij"
            //       << endl;

            // Calculating pij for new sender to find min Pij
            Coord *senderCoord = new Coord(); // For sender
            senderCoord = &(msg->FloodingMessage::getOriginPosition());

            nodeix = senderCoord->x;
            nodeiy = senderCoord->y;
            nodeiz = senderCoord->z;

            //std::cout << "Sender position: " << nodeix << " " << nodeiy << " "
            // << nodeiz << " " << endl;

            curPosition = mobility->getPositionAt(simTime()); // For Receiver
            msg->setOriginPosition(curPosition);
            nodejx = curPosition.x;
            nodejy = curPosition.y;
            nodejz = curPosition.z;

            // std::cout << "Receiver position: " << nodejx << " " << nodejy << " "
            //<< nodejz << " " << endl;

            // 1. Distance:
            dist = sqrt(
                    pow(nodeix - nodejx, 2) + pow(nodeiy - nodejy, 2)
                            + pow(nodeiz - nodejz, 2));
            double pijNew = dist / interferenceRangeR;

            if (pijNew < pijMin) {
                pijMin = pijNew;
            }

            //std::cout << "Distance: " << dist << " 2nd case Pij: " << pij
            //<< " pijmin: " << pijMin << endl;

            // Calculation over.
            //simtime_t delay = uniform(0, 0.1);
            if ((pijMin <= cutOffNew && pijMin > 0.005) || pijMin > cutOff) {
                sendDelayedDown(msg->dup(), delay);
                std::cout << "SENT 2nd case pijmin: " << pijMin << endl;
                numSent++;
            } else {
                std::cout << "NOT sent pijmin: " << pijMin << endl;
            }
        }
    }
}

void WeightedPPersistant::handleMessage(cMessage *msg) {
    FloodingBase::handleMessage(msg);
}
void WeightedPPersistant::handleSelfMsg(cMessage *msg) {
    FloodingBase::handleSelfMsg(msg);

}

void WeightedPPersistant::handlePositionUpdate(cObject *obj) {
    DemoBaseApplLayer::handlePositionUpdate(obj);
    curPosition = mobility->getPositionAt(simTime());

// the vehicle has moved. Code that reacts to new positions goes here.
// member variables such as currentPosition and currentSpeed are updated in
// the parent class

// our code (define node j here)

    nodejx = curPosition.x;
    nodejy = curPosition.y;
    nodejz = curPosition.z;
}
