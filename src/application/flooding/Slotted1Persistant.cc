//
// This program is free software: you can resdistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is sdistributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Slotted1Persistant.h"
#include <algorithm>
#include <cmath>
#include <vector>

Define_Module(Slotted1Persistant);

double snodejx;
double snodejy;
double snodejz;
double snodeix;
double snodeiy;
double snodeiz;
double sinterferenceRangeR = 100, sdist = 0;
double tsij = 0;
const double hop_delay_time = 2;
int sij = 0;
const int numberSlots = 20;
std::set<int> smsgIdList;
std::set<int> sdupList;

long snumSent = 0;
long snumDuplicates = 0;
bool snewFirstMessage = false;
bool sisDuplicate = false;
long snumfreshMessage = 0;

int Slotted1Persistant::overallReachedNodes;

void Slotted1Persistant::initialize(int stage) {
    FloodingBase::initialize(stage);
    if (stage == 0) {
        //Initializing members and pointers of your application goes here
        curPosition = mobility->getPositionAt(simTime()); // set current position during initialization
        //overallReachedNodes = 0;
    } else if (stage == 1) {
        //Initializing members that require initialized other modules goes here
    }
}

void Slotted1Persistant::finish() {
    FloodingBase::finish();
    //maybe you want to record some scalars?

    recordScalar("#overallReachedNodes", overallReachedNodes);
    recordScalar("#newReachedNodes", snumfreshMessage);
    recordScalar("#duplicates", snumDuplicates);
    recordScalar("#snumSent", snumSent);

//    int pid = getParentModule()->getIndex();
//    std::cout << "MAC ID: " << pid << " Number sent: " << snumSent << '\n';

}

void Slotted1Persistant::handleFloodingMsg(FloodingMessage *msg) {
    FloodingBase::handleFloodingMsg(msg);

    overallReachedNodes++;
    reachedNodes.record(overallReachedNodes);

    int id = getParentModule()->getIndex();
    std::cout << "MAC: " << id << endl;

    simtime_t NewReceptionTime = simTime(); // get simTime() when message is received

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
    if ((!smsgIdList.empty() && smsgIdList.find(id) == smsgIdList.end())
            || smsgIdList.empty()) { // check for duplicate MAC ID
        smsgIdList.insert(id);                      // new ID
        snewFirstMessage = true;

        snumfreshMessage++;
        freshMessage.record(id);

    } else {
        // already exists- duplicate message
        snewFirstMessage = false;

        snumDuplicates++;
        duplicates.record(id);

        sdupList.insert(id); // insert MAC ID, if message received for 2nd time
    }

    // Case 1: Message received for the 1st time, send
    // without any further delay
    if (snewFirstMessage) {
        // Just for checking - print message
        std::cout << "MAC ID List: " << endl;

        for (std::set<int>::iterator s = smsgIdList.begin();
                s != smsgIdList.end(); s++) {
            std::cout << *s << "  ";
        }
        std::cout << endl;

        Coord *senderCoord = new Coord(); // For sender
        senderCoord = &(msg->FloodingMessage::getOriginPosition());

        snodeix = senderCoord->x;
        snodeiy = senderCoord->y;
        snodeiz = senderCoord->z;

//        std::cout << "Sender position: " << snodeix << " " << snodeiy << " "
//                << snodeiz << " " << endl;

        curPosition = mobility->getPositionAt(simTime()); // For Receiver
        msg->setOriginPosition(curPosition);
        snodejx = curPosition.x;
        snodejy = curPosition.y;
        snodejz = curPosition.z;

//        std::cout << "Receiver position: " << snodejx << " " << snodejy << " "
//                << snodejz << " " << endl;

        // 1. sdistance:
        sdist = sqrt(
                pow(snodeix - snodejx, 2) + pow(snodeiy - snodejy, 2)
                        + pow(snodeiz - snodejz, 2));
        if (sdist <= sinterferenceRangeR) {
            sij = numberSlots * (1 - sdist / sinterferenceRangeR);
            tsij = sij * hop_delay_time;

            std::cout << "SENT sdistance: " << sdist << "\t sij " << sij
                    << "\t tsij " << tsij << endl;

            sendDelayedDown(msg->dup(), tsij);  // send message after Tsij
            sentMessages.record(id);
            snumSent++;
        }
        // TODO: The following code block might help you - the statistics are wrong
        // at the moment

        msg->setOriginPosition(curPosition);

        //snumSent++;
    }
    // case 1 over
    else { // case 2 begins when duplicate message received
        std::cout << "***Discard: \n" << endl;
    }
}

void Slotted1Persistant::handleMessage(cMessage *msg) {
    FloodingBase::handleMessage(msg);
}
void Slotted1Persistant::handleSelfMsg(cMessage *msg) {
    FloodingBase::handleSelfMsg(msg);
//this method is for self messages (mostly timers)
//it is important to call the DemoBaseApplLayer function for BSM and WSM transmission

}

void Slotted1Persistant::handlePositionUpdate(cObject *obj) {
    DemoBaseApplLayer::handlePositionUpdate(obj);
    curPosition = mobility->getPositionAt(simTime());
//the vehicle has moved. Code that reacts to new positions goes here.
//member variables such as currentPosition and currentSpeed are updated in the parent class

    snodejx = curPosition.x;
    snodejy = curPosition.y;
    snodejz = curPosition.z;
}
