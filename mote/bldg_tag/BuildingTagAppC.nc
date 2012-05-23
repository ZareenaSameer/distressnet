/*
 * Copyright (c) 2007-2020 Texas A&M University System
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holders nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/* BuildingTagC.nc
* - main control for the building tag mote
* Basic capabilities:
* - report information when queried
* - be programmable with new information as required
* 
*/

//	NEW: NOW CONTAINS SINK ELECTION AND RPL


configuration BuildingTagAppC { }

implementation 
{
	//			--------BOILERPLATE AND RPL--------
	
	components MainC, BuildingTagC as App;
	App.Boot -> MainC;
	
	//	blip?
	components IPStackC;
	components IPDispatchC;
	components IPProtocolsP;
	App.SplitControl -> IPStackC;

	//	rpl?
	components RPLRankC;
	components RPLRoutingEngineC;
	components RPLDAORoutingEngineC;
	components RPLRoutingC;
	App.RPLDAO -> RPLDAORoutingEngineC;
	App.RPLRoute -> RPLRoutingEngineC;
	App.RootControl -> RPLRoutingEngineC;
	App.RoutingControl -> RPLRoutingEngineC;
	
	components IPForwardingEngineP;
	App.RoutingTable -> IPForwardingEngineP.ForwardingTable;

	//			--------TIMERS--------	
	components new TimerMilliC() as ElectionTimer; 
	App.ElectionTimer -> ElectionTimer;

	components new TimerMilliC() as BatteryVoltageTimer; 
	App.BatteryVoltageTimer -> BatteryVoltageTimer;

	components new TimerMilliC() as DataTimer; 
	App.DataTimer -> DataTimer;
	
	components new TimerMilliC() as BroadTimer; 
	App.BroadTimer -> BroadTimer;

	components new TimerMilliC() as SendTimer; 
	App.SendTimer -> SendTimer;

// 	components new TimerMilliC() as TestTimer; 
// 	App.TestTimer -> TestTimer;
	
	components new TimerMilliC() as ResetBeaconTimer; 
	App.ResetBeaconTimer -> ResetBeaconTimer;
	
	components new TimerMilliC() as RootSelectionTimer; 
	App.RootSelectionTimer -> RootSelectionTimer;
	
	
	//			--------SOCKETS AND NETWORK--------
	components new UdpSocketC() as DataUDP;
	App.BtagDataUDP -> DataUDP;
	
	components new UdpSocketC() as ElectionUDP;
	App.ElectionUDP -> ElectionUDP;
	
	components new UdpSocketC() as ReportUDP;
	App.BeaconRecvUDP -> ReportUDP;
	
	components new UdpSocketC() as ProgrammerUDP;
	App.ProgrammerUDP -> ProgrammerUDP;
	
	components new UdpSocketC() as RootSelectUDP;
	App.RootSelectUDP -> RootSelectUDP;
	

	//			--------MISC--------
	components LedsC;
	App.Leds -> LedsC;
	
	components new DemoSensorC();
	App.BatteryVoltageRead -> DemoSensorC;
	
	components RandomC;
	App.Random -> RandomC;

	
	#ifdef PRINTFUART_ENABLED
		components PrintfC;
		components SerialStartC;
	#endif

}
