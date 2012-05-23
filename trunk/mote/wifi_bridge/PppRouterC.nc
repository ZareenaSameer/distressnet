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

#include <iprouting.h>

#include "ppp.h"

configuration PppRouterC {
} implementation {
  components PppRouterP;

  components MainC;
  PppRouterP.Boot -> MainC;

  components LedsC as LedsC;
  PppRouterP.Leds -> LedsC;

  components PppDaemonC;
  PppRouterP.PppControl -> PppDaemonC;

  components PppIpv6C;
  PppDaemonC.PppProtocol[PppIpv6C.ControlProtocol] -> PppIpv6C.PppControlProtocol;
  PppDaemonC.PppProtocol[PppIpv6C.Protocol] -> PppIpv6C.PppProtocol;
  PppIpv6C.Ppp -> PppDaemonC;
  PppIpv6C.LowerLcpAutomaton -> PppDaemonC;

  PppRouterP.Ipv6LcpAutomaton -> PppIpv6C;
  PppRouterP.PppIpv6 -> PppIpv6C;
  PppRouterP.Ppp -> PppDaemonC;

#if defined(PLATFORM_TELOSB) || defined(PLATFORM_EPIC)
  components PlatformHdlcUartC as HdlcUartC;
#else
  components DefaultHdlcUartC as HdlcUartC;
#endif
  PppDaemonC.HdlcUart -> HdlcUartC;
  PppDaemonC.UartControl -> HdlcUartC;

  // SDH : don't bother including the PppPrintfC by default
  // components PppPrintfC, PppC;;
  // PppPrintfC.Ppp -> PppDaemonC;
  // PppDaemonC.PppProtocol[PppPrintfC.Protocol] -> PppPrintfC;
  // PppPrintfC.Ppp -> PppC;

  components IPStackC, IPForwardingEngineP, IPPacketC;
  IPForwardingEngineP.IPForward[ROUTE_IFACE_PPP] -> PppRouterP.IPForward;
  PppRouterP.IPControl -> IPStackC;
  PppRouterP.ForwardingTable -> IPStackC;
  PppRouterP.IPPacket -> IPPacketC;

#ifdef RPL_ROUTING
  components RPLRoutingC, RplBorderRouterP;
  PppRouterP.RootControl -> RPLRoutingC;
  RplBorderRouterP.ForwardingEvents -> IPStackC.ForwardingEvents[ROUTE_IFACE_PPP];
  RplBorderRouterP.IPPacket -> IPPacketC;
#endif

  // UDP shell on port 2000
//   components UDPShellC;

  // prints the routing table
//   components RouteCmdC;

#ifdef PRINTFUART_ENABLED
	components PrintfC;
	components SerialStartC;
	#endif
	

#ifndef IN6_PREFIX
  components Dhcp6ClientC;
  PppRouterP.Dhcp6Info -> Dhcp6ClientC;
#endif

	components new UdpSocketC() as myProxyComms;
	PppRouterP.myProxyComms -> myProxyComms;
	
	components new UdpSocketC() as BtagCollectComms;
	PppRouterP.BtagCollectComms -> BtagCollectComms;
	
	components new TimerMilliC() as BeaconTimer; 
	PppRouterP.BeaconTimer -> BeaconTimer;

	components new UdpSocketC() as BeaconComms;
	PppRouterP.BeaconComms -> BeaconComms;
	
}
