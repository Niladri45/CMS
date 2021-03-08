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

package simpleflooding.application.flooding;
import org.car2x.veins.base.modules.IBaseApplLayer;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

simple FloodingBase extends DemoBaseApplLayer like IBaseApplLayer {   
    parameters:
        bool debug = default(false); //whether to print debug output
        int packetLength = default(1024bit) @unit("bit"); //packet length of the application
        int packetPriority = default(3); // priority: 3=AC_VO, 2=AC_VI, 1=AC_BE, 0=AC_BK       
        bool sendWhileParking  = default(false); //send messages when the car is parked
        @class(FloodingRSU);

}
