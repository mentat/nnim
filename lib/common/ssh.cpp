// --*-c++-*--
/*
    $Id: ssh.cpp,v 1.1 2002/06/06 17:21:54 thementat Exp $
 
    GNU Messenger - The secure instant messenger
    Copyright (C) 2001  Jesse Lovelace
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    -----
    $Log: ssh.cpp,v $
    Revision 1.1  2002/06/06 17:21:54  thementat
    Initial revision

    Revision 1.2  2001/10/06 16:53:20  thementat
    Added GNU text, not newest branch.

*/

SecByteBlock gmSSH::MakeBinaryPacketNoMAC(const SecByteBlock& payload,
                                              const SecByteBlock& random_padding, 
                                              const byte padding_length)
{
/*
Each packet is in the following format:

  uint32    packet_length
  byte      padding_length
  byte[n1]  payload; n1 = packet_length - padding_length - 1
  byte[n2]  random padding; n2 = padding_length
  byte[m]   mac (message authentication code); m = mac_length
*/ 
  
  unsigned char len[4];
  unsigned long packet_length = payload_length + padding_length + 1;
  unsigned long total_length = 4 + packet_length;

  len[0] = (unsigned char) (packet_length & 0xff);
  len[1] = (unsigned char) ((packet_length >> 8) & 0xff);
  len[2] = (unsigned char) ((packet_length >> 16) & 0xff);
  len[3] = (unsigned char) ((packet_length >> 24) & 0xff);

  unsigned long count = 0;
  SecByteBlock packet(total_length);
    
  packet[count++] = len[0];
  packet[count++] = len[1];
  packet[count++] = len[2];
  packet[count++] = len[3];
  packet[count++] = padding_length;

  unsigned long it = 0;

  for (it = 0; it < payload_length; it++)
  {
    packet[count++] = payload[it];
  }

  for (it = 0; it < padding_length; it++)
  {
    packet[count++] = random_padding[it];
  }

  assert(count == total_length);

  return packet;

}

SecByteBlock gmSSH::EncryptPacket(const SecByteBlock& packet_minus_mac)
{





}

SecByteBlock gmSSH::MakeNegotiationPacket()
{


  /*

Key exchange begins by each side sending the following packet:

  byte      SSH_MSG_KEXINIT
  byte[16]  cookie (random bytes)
  string    kex_algorithms
  string    server_host_key_algorithms
  string    encryption_algorithms_client_to_server
  string    encryption_algorithms_server_to_client
  string    mac_algorithms_client_to_server
  string    mac_algorithms_server_to_client
  string    compression_algorithms_client_to_server
  string    compression_algorithms_server_to_client
  string    languages_client_to_server
  string    languages_server_to_client
  boolean   first_kex_packet_follows
  uint32    0 (reserved for future extension)

  */

  return 0;
}

SecByteBlock gmSSH::MakeDisconnectPacket(unsigned long reason_code, const char * description, 
                                              const char * lang_code)
{
/*
  byte      SSH_MSG_DISCONNECT
  uint32    reason code
  string    description [RFC-2279]
  string    language tag [RFC-1766]

This message causes immediate termination of the connection.  All
implementations MUST be able to process this message; they SHOULD be
able to send this message.
*/

  return 0;
}



SecByteBlock gmSSH::MakeServiceRequestPacket(const char * service_name)
{
/*
After the key exchange, the client requests a service. The service is
identified by a name. The format of names and procedures for defining
new names are defined in [SSH-ARCH].

Currently, the following names have been reserved:

  ssh-userauth
  ssh-connection

Similar local naming policy is applied to the service names, as is
applied to the algorithm names; a local service should use the
"servicename@domain" syntax.

  byte      SSH_MSG_SERVICE_REQUEST
  string    service name

If the server rejects the service request, it SHOULD send an appropriate
SSH_MSG_DISCONNECT message and MUST disconnect.
*/

  return 0;
}

SecByteBlock gmSSH::MakeMAC(const SecByteBlock& packet)
{
  
  return 0;

}
