//==============================================================================
//
//  This file is part of GNSSTk, the GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2021, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024 
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public 
//                            release, distribution is unlimited.
//
//==============================================================================

#ifndef TCPSTREAMBUFF_HPP
#define TCPSTREAMBUFF_HPP

#include <sstream>
#include <errno.h>
#include <stdlib.h>

#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "FDStreamBuff.hpp"

namespace gnsstk
{
   class SocketAddr;

   class IPaddress
   {
      friend class SocketAddr;
      
      unsigned long address;                // Address: 4 bytes in the network byte order
      IPaddress(const unsigned int netaddr) : address(netaddr) {}

   public:
      IPaddress() : address(INADDR_ANY) {}   // Wildcard address
      IPaddress(const std::string& name);         // Involves the name resolution
      unsigned long net_addr() const { return address; }
  
      friend std::ostream& operator <<(std::ostream& os, const IPaddress addr);
      friend std::ostream& operator <<(std::ostream& os, const SocketAddr& addr);
   };

   class SocketAddr : sockaddr_in
   {
      friend class StreamSocket;
      friend class UDPsocketIn;
      SocketAddr() {}
      
   public:
      SocketAddr(const IPaddress host, const short port_no);
      operator sockaddr * () const      { return (sockaddr *)this; }
      friend std::ostream& operator <<(std::ostream& os, const SocketAddr& addr);
   };


   class TCPStreamBuff : public FDStreamBuff
   {
   public:
      TCPStreamBuff() : FDStreamBuff(-1) {}

      ~TCPStreamBuff() {close();}

      int connect(const SocketAddr target_address);

      // Take a file handle (which is supposed to be a listening socket), 
      // accept a connection if any,  and return the corresponding TCPbuf
      // for that connection. On exit, peeraddr would be an addr of the
      // connected peer
      int accept(int listening_socket, SocketAddr& peeraddr);

   };

} // end of namespace
#endif
