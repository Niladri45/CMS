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

#ifndef __SIMPLEFLOODING_FLOODINGRSU_H_
#define __SIMPLEFLOODING_FLOODINGRSU_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/DemoBaseApplLayer.h"
#include "veins/modules/messages/DemoServiceAdvertisement_m.h"
#include "messages/FloodingMessage_m.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"


using veins::DemoBaseApplLayer;
using veins::Coord;
using veins::BaseFrame1609_4;

class FloodingRSU : public DemoBaseApplLayer {
public:
    FloodingRSU();
    ~FloodingRSU();
    protected:
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

    protected:
        cMessage* startFlooding;
};

#endif
