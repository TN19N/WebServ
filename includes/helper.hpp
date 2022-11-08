#ifndef __HELPER_HPP__
#define __HELPER_HPP__

#include <unordered_map>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>
#include <string>

std::unordered_map<int, std::string> familyType = {
	{AF_UNSPEC, "AF_UNSPEC"},
	{AF_UNIX, "AF_UNIX"},
	{AF_LOCAL, "AF_LOCAL"},
	{AF_INET, "AF_INET"},
	{AF_IMPLINK, "AF_IMPLINK"},
	{AF_PUP, "AF_PUP"},
	{AF_CHAOS, "AF_CHAOS"},
	{AF_NS, "AF_NS"},
	{AF_ISO, "AF_ISO"},
	{AF_OSI, "AF_OSI"},
	{AF_ECMA, "AF_ECMA"},
	{AF_DATAKIT, "AF_DATAKIT"},
	{AF_CCITT, "AF_CCITT"},
	{AF_SNA, "AF_SNA"},
	{AF_DECnet, "AF_DECnet"},
	{AF_DLI, "AF_DLI"},
	{AF_LAT, "AF_LAT"},
	{AF_HYLINK, "AF_HYLINK"},
	{AF_APPLETALK, "AF_APPLETALK"},
	{AF_ROUTE, "AF_ROUTE"},
	{AF_LINK, "AF_LINK"},
	{pseudo_AF_XTP, "pseudo_AF_XTP"},
	{AF_COIP, "AF_COIP"},
	{AF_CNT, "AF_CNT"},
	{pseudo_AF_RTIP, "pseudo_AF_RTIP"},
	{AF_IPX, "AF_IPX"},
	{AF_SIP, "AF_SIP"},
	{pseudo_AF_PIP, "pseudo_AF_PIP"},
	{AF_NDRV, "AF_NDRV"},
	{AF_ISDN, "AF_ISDN"},
	{AF_E164, "AF_E164"},
	{pseudo_AF_KEY, "pseudo_AF_KEY"},
	{AF_INET6, "AF_INET6"},
	{AF_NATM, "AF_NATM"},
	{AF_SYSTEM, "AF_SYSTEM"},
	{AF_NETBIOS, "AF_NETBIOS"},
	{AF_PPP, "AF_PPP"},
	{pseudo_AF_HDRCMPLT, "pseudo_AF_HDRCMPLT"},
	{AF_RESERVED_36, "AF_RESERVED_36"},
	{AF_IEEE80211, "AF_IEEE80211"},
	{AF_UTUN, "AF_UTUN"},
	{AF_MAX, "AF_MAX"}
};

std::unordered_map<int, std::string> socketType = {
	{SOCK_STREAM, "SOCK_STREAM"},
	{SOCK_DGRAM, "SOCK_DGRAM"},
	{SOCK_RAW, "SOCK_RAW"},
	{SOCK_RDM, "SOCK_RDM"},
	{SOCK_SEQPACKET, "SOCK_SEQPACKET"}
};

std::unordered_map<int, std::string> protocolType = {
	{IPPROTO_IP, "IPPROTO_IP"},
	{IPPROTO_ICMP, "IPPROTO_ICMP"},
	{IPPROTO_IGMP, "IPPROTO_IGMP"},
	{IPPROTO_GGP, "IPPROTO_GGP"},
	{IPPROTO_IPV4, "IPPROTO_IPV4"},
	{IPPROTO_IPIP, "IPPROTO_IPIP"},
	{IPPROTO_TCP, "IPPROTO_TCP"},
	{IPPROTO_ST, "IPPROTO_ST"},
	{IPPROTO_EGP, "IPPROTO_EGP"},
	{IPPROTO_PIGP, "IPPROTO_PIGP"},
	{IPPROTO_RCCMON, "IPPROTO_RCCMON"},
	{IPPROTO_NVPII, "IPPROTO_NVPII"},
	{IPPROTO_PUP, "IPPROTO_PUP"},
	{IPPROTO_ARGUS, "IPPROTO_ARGUS"},
	{IPPROTO_EMCON, "IPPROTO_EMCON"},
	{IPPROTO_XNET, "IPPROTO_XNET"},
	{IPPROTO_CHAOS, "IPPROTO_CHAOS"},
	{IPPROTO_UDP, "IPPROTO_UDP"},
	{IPPROTO_MUX, "IPPROTO_MUX"},
	{IPPROTO_MEAS, "IPPROTO_MEAS"},
	{IPPROTO_HMP, "IPPROTO_HMP"},
	{IPPROTO_PRM, "IPPROTO_PRM"},
	{IPPROTO_IDP, "IPPROTO_IDP"},
	{IPPROTO_TRUNK1, "IPPROTO_TRUNK1"},
	{IPPROTO_TRUNK2, "IPPROTO_TRUNK2"},
	{IPPROTO_LEAF1, "IPPROTO_LEAF1"},
	{IPPROTO_LEAF2, "IPPROTO_LEAF2"},
	{IPPROTO_RDP, "IPPROTO_RDP"},
	{IPPROTO_IRTP, "IPPROTO_IRTP"},
	{IPPROTO_TP, "IPPROTO_TP"},
	{IPPROTO_BLT, "IPPROTO_BLT"},
	{IPPROTO_NSP, "IPPROTO_NSP"},
	{IPPROTO_INP, "IPPROTO_INP"},
	{IPPROTO_SEP, "IPPROTO_SEP"},
	{IPPROTO_3PC, "IPPROTO_3PC"},
	{IPPROTO_IDPR, "IPPROTO_IDPR"},
	{IPPROTO_XTP, "IPPROTO_XTP"},
	{IPPROTO_DDP, "IPPROTO_DDP"},
	{IPPROTO_CMTP, "IPPROTO_CMTP"},
	{IPPROTO_TPXX, "IPPROTO_TPXX"},
	{IPPROTO_IL, "IPPROTO_IL"},
	{IPPROTO_IPV6, "IPPROTO_IPV6"},
	{IPPROTO_SDRP, "IPPROTO_SDRP"},
	{IPPROTO_ROUTING, "IPPROTO_ROUTING"},
	{IPPROTO_FRAGMENT, "IPPROTO_FRAGMENT"},
	{IPPROTO_IDRP, "IPPROTO_IDRP"},
	{IPPROTO_RSVP, "IPPROTO_RSVP"},
	{IPPROTO_GRE, "IPPROTO_GRE"},
	{IPPROTO_MHRP, "IPPROTO_MHRP"},
	{IPPROTO_BHA, "IPPROTO_BHA"},
	{IPPROTO_ESP, "IPPROTO_ESP"},
	{IPPROTO_AH, "IPPROTO_AH"},
	{IPPROTO_INLSP, "IPPROTO_INLSP"},
	{IPPROTO_SWIPE, "IPPROTO_SWIPE"},
	{IPPROTO_NHRP, "IPPROTO_NHRP"},
	{IPPROTO_ICMPV6, "IPPROTO_ICMPV6"},
	{IPPROTO_NONE, "IPPROTO_NONE"},
	{IPPROTO_DSTOPTS, "IPPROTO_DSTOPTS"},
	{IPPROTO_AHIP, "IPPROTO_AHIP"},
	{IPPROTO_CFTP, "IPPROTO_CFTP"},
	{IPPROTO_HELLO, "IPPROTO_HELLO"},
	{IPPROTO_SATEXPAK, "IPPROTO_SATEXPAK"},
	{IPPROTO_KRYPTOLAN, "IPPROTO_KRYPTOLAN"},
	{IPPROTO_RVD, "IPPROTO_RVD"},
	{IPPROTO_IPPC, "IPPROTO_IPPC"},
	{IPPROTO_ADFS, "IPPROTO_ADFS"},
	{IPPROTO_SATMON, "IPPROTO_SATMON"},
	{IPPROTO_VISA, "IPPROTO_VISA"},
	{IPPROTO_IPCV, "IPPROTO_IPCV"},
	{IPPROTO_CPNX, "IPPROTO_CPNX"},
	{IPPROTO_CPHB, "IPPROTO_CPHB"},
	{IPPROTO_WSN, "IPPROTO_WSN"},
	{IPPROTO_PVP, "IPPROTO_PVP"},
	{IPPROTO_BRSATMON, "IPPROTO_BRSATMON"},
	{IPPROTO_ND, "IPPROTO_ND"},
	{IPPROTO_WBMON, "IPPROTO_WBMON"},
	{IPPROTO_WBEXPAK, "IPPROTO_WBEXPAK"},
	{IPPROTO_EON, "IPPROTO_EON"},
	{IPPROTO_VMTP, "IPPROTO_VMTP"},
	{IPPROTO_SVMTP, "IPPROTO_SVMTP"},
	{IPPROTO_VINES, "IPPROTO_VINES"},
	{IPPROTO_TTP, "IPPROTO_TTP"},
	{IPPROTO_IGP, "IPPROTO_IGP"},
	{IPPROTO_DGP, "IPPROTO_DGP"},
	{IPPROTO_TCF, "IPPROTO_TCF"},
	{IPPROTO_IGRP, "IPPROTO_IGRP"},
	{IPPROTO_OSPFIGP, "IPPROTO_OSPFIGP"},
	{IPPROTO_SRPC, "IPPROTO_SRPC"},
	{IPPROTO_LARP, "IPPROTO_LARP"},
	{IPPROTO_MTP, "IPPROTO_MTP"},
	{IPPROTO_AX25, "IPPROTO_AX25"},
	{IPPROTO_IPEIP, "IPPROTO_IPEIP"},
	{IPPROTO_MICP, "IPPROTO_MICP"},
	{IPPROTO_SCCSP, "IPPROTO_SCCSP"},
	{IPPROTO_ETHERIP, "IPPROTO_ETHERIP"},
	{IPPROTO_ENCAP, "IPPROTO_ENCAP"},
	{IPPROTO_APES, "IPPROTO_APES"},
	{IPPROTO_GMTP, "IPPROTO_GMTP"},
	{IPPROTO_PIM, "IPPROTO_PIM"},
	{IPPROTO_IPCOMP, "IPPROTO_IPCOMP"},
	{IPPROTO_PGM, "IPPROTO_PGM"},
	{IPPROTO_SCTP, "IPPROTO_SCTP"},
	{IPPROTO_DIVERT, "IPPROTO_DIVERT"},
	{IPPROTO_RAW, "IPPROTO_RAW"}
};

void printInfo(struct addrinfo* ptr)
{
	std::cout << "-------------------------------" << std::endl;
	std::cout << "ai_flags: " << ptr->ai_flags << std::endl;
	std::cout << "ai_family: " << "(PF)"+familyType[ptr->ai_family] << std::endl;
	std::cout << "ai_socktype: " << socketType[ptr->ai_socktype] << std::endl;
	std::cout << "ai_protocol: " << protocolType[ptr->ai_protocol] << std::endl;
	std::cout << "ai_addrlen: " << ptr->ai_addrlen << " bytes \n";
	std::cout << "ai_canonname: " << (ptr->ai_canonname ? ptr->ai_canonname : "NULL") << std::endl;
	std::cout << "ai_addr: \n";
	if (ptr->ai_family == PF_INET)
	{
		char str[INET_ADDRSTRLEN];
		struct sockaddr_in* sock = (struct sockaddr_in *)ptr->ai_addr;
		std::cout << "	-> sin_len: " << (int)sock->sin_len << " bytes\n";
		std::cout << "	-> sin_family: " << familyType[sock->sin_family] << std::endl;
		std::cout << "	-> sin_port: " << sock->sin_port << std::endl;
		inet_ntop(sock->sin_family, &(sock->sin_addr), str, INET_ADDRSTRLEN);
		std::cout << "	-> sin_addr: " << str << std::endl;
	}
	else if (ptr->ai_family == PF_INET6)
	{
		char str[INET6_ADDRSTRLEN];
		struct sockaddr_in6* sock = (struct sockaddr_in6 *)ptr->ai_addr;
		std::cout << "	-> sin6_len: " << (int)sock->sin6_len << " bytes \n";
		std::cout << "	-> sin_family: " << familyType[sock->sin6_family] << std::endl;
		std::cout << "	-> sin6_port: " << sock->sin6_port << std::endl;
		std::cout << "	-> sin6_flowinfo: " << sock->sin6_flowinfo << std::endl;
		std::cout << "	-> sin6_scope_id: " << sock->sin6_scope_id << std::endl;
		inet_ntop(sock->sin6_family, &(sock->sin6_addr), str, INET6_ADDRSTRLEN);
		std::cout << "	-> sin6_addr: " << str << std::endl;
	}
	std::cout << "-------------------------------" << std::endl;
}

#endif