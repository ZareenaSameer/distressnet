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

/**
 * When packets leave a RPL domain, we're need to remove and RPL
 * headers which have been inserted and/or reencapsulate the packet.
 * This component hooks into the forwarding path to do this by
 * converting any RPL TLV options in IPv6 hop-by-hop options header to
 * PadN options.
 *
 * @author Stephen Dawson-Haggerty <stevedh@eecs.berkeley.edu>
 */

#include <lib6lowpan/ip.h>
#include <iprouting.h>
#include <RPL.h>

module RplBorderRouterP {
  uses {
    interface ForwardingEvents;
    interface IPPacket;
  }
} implementation {

  event bool ForwardingEvents.initiate(struct ip6_packet *pkt,
                                       struct in6_addr *next_hop) {
    return TRUE;
  }

  event bool ForwardingEvents.approve(struct ip6_packet *pkt,
                                      struct in6_addr *next_hop) {
    int off;
    uint8_t nxt = IPV6_HOP;
    if (pkt->ip6_inputif == ROUTE_IFACE_PPP)
      return FALSE;

    /* remove any RPL options in the hop-by-hop header by converting
       them to a PadN option */
    off = call IPPacket.findHeader(pkt->ip6_data, pkt->ip6_hdr.ip6_nxt, &nxt);
    if (off < 0) return TRUE;
    call IPPacket.delTLV(pkt->ip6_data, off, RPL_HBH_RANK_TYPE);

    return TRUE;
  }

  event void ForwardingEvents.linkResult(struct in6_addr *dest, struct send_info *info) {

  }
}
