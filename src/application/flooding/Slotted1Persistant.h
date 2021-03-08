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

#ifndef __SIMPLEFLOODING_SLOTTED1PERSISTANT_H_
#define __SIMPLEFLOODING_SLOTTED1PERSISTANT_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "messages/FloodingMessage_m.h"
#include "FloodingBase.h"

using veins::DemoBaseApplLayer;
using veins::Coord;

class Slotted1Persistant : public FloodingBase {
public:
    virtual void finish();
protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

    virtual void handleSelfMsg(cMessage* msg);
    virtual void handlePositionUpdate(cObject* obj);
    virtual void handleFloodingMsg(FloodingMessage* msg);

protected:
    static int overallReachedNodes;
};

#endif
