#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include "checksum.h"
#include "print.h"

#ifndef ETHERTYPE_IPV6
#define ETHERTYPE_IPV6 0x86dd
#endif

int AnalyzeArp(u_char *data, int size) {
  u_char *ptr;
  int lest;
  struct ether_arp *arp;

  ptr = data;
  lest = size;

  if (lest < sizeof(struct ether_arp)) {
    fprintf(stderr, "lest(%d) < sizeof(struct iphdr) \n", lest);
    return -1;
  }

  arp = (struct ether_arp *)ptr;
  ptr += sizeof(struct ether_arp);
  lest -= sizeof(struct ether_arp);

  PrintArp(arp, stdout);

  return 0;
}


int AnalyzeIcmp(u_char *data, int size) {
  u_char *ptr;
  int lest;
  struct icmp *icmp;

  ptr = data;
  lest = size;

  if (lest < sizeof(struct icmp)) {
    fprintf(stderr, "lest(%d) < sizeof(struct icmp) \n", lest);
    return -1;
  }

  icmp = (struct icmp *)ptr;
  ptr += sizeof(struct icmp);
  lest -= sizeof(struct icmp);

  PrintIcmp(icmp, stdout);

  return 0;
}

int AnalyzeIcmp6(u_char *data, int size) {
  u_char *ptr;
  int lest;
  struct icmp6_hdr *icmp6;

  ptr = data;
  lest = size;

  if (lest < sizeof(struct icmp6_hdr)) {
    fprintf(stderr, "lest(%d) < sizeof(struct icmp6_hdr) \n", lest);
    return -1;
  }

  icmp6 = (struct icmp6_hdr *)ptr;
  ptr += sizeof(struct icmp6_hdr);
  lest -= sizeof(struct icmp6_hdr);

  PrintIcmp6(icmp6, stdout);

  return 0;
}


int AnalyzeTcp(u_char *data, int size) {
  u_char *ptr;
  int lest;
  struct tcphdr *tcphdr;

  ptr = data;
  lest = size;

  if (lest < sizeof(struct tcphdr)) {
    fprintf(stderr, "lest(%d) < sizeof(struct tcphdr) \n", lest);
    return -1;
  }

  tcphdr = (struct tcphdr *)ptr;
  ptr += sizeof(struct tcphdr);
  lest -= sizeof(struct tcphdr);

  PrintTcp(tcphdr, stdout);

  return 0;
}

int AnalyzeUdp(u_char *data, int size) {
  u_char *ptr;
  int lest;
  struct udphdr *udphdr;

  ptr = data;
  lest = size;

  if (lest < sizeof(struct udphdr)) {
    fprintf(stderr, "lest(%d) < sizeof(struct udphdr) \n", lest);
    return -1;
  }

  udphdr = (struct udphdr *)ptr;
  ptr += sizeof(struct udphdr);
  lest -= sizeof(struct udphdr);

  PrintUdp(udphdr, stdout);

  return 0;
}

int AnalyzeIp(u_char *data, int size) {
  u_char *ptr;
  int lest;
  struct iphdr *iphdr;
  u_char *option;
  int optionLen, len;
  unsigned short sum;

  ptr = data;
  lest = size;

  if (lest < sizeof(struct iphdr)) {
    fprintf(stderr, "lest(%d) < sizeof(struct iphdr) \n", lest);
    return -1;
  }

  iphdr = (struct iphdr *)ptr;
  ptr += sizeof(struct iphdr);
  lest -= sizeof(struct iphdr);

  optionLen = iphdr -> ihl * 4 - sizeof(struct iphdr);

  if (optionLen > 0) {
         if (optionLen > 1500) {
                fprintf(stderr, "IP optionLen(%d) : too big\n", optionLen);
                return -1;
         }

         option = ptr;
         ptr += optionLen;
         lest -= optionLen
  }

  if (checkIPchecksum(iphdr, option, optionLen) == 0) {
         fprintf(stderr, "bad ip checksum\n");
         return -1;
 }

 PrintIpHeader(iphdr, option, optionLen, stdout);

 if (iphdr -> protocol == IPPROTO_ICMP) {
        len = ntohs(iphdr -> tot_len) - iphdr -> ihl * 4;
        sum = checksum(ptr, len);
        if (sum != 0 && sum != 0xFFFF) {
               fprintf(stderr, "bad icmp checksum\n");
               return -1;
        }
        AnalyzeIcmp(ptr, lest);
}
}
 }

  return 0;
}
