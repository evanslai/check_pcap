#include <stdio.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include "../ether_proto.h"
#include "../utils.h"
#include "8021q.h"


static int vlan_handler(struct pkt_buff *pb)
{
	struct vlan_hdr *vlanh;
	int ret = 0;
	unsigned int len;
	unsigned short proto;

	vlanh = (struct vlan_hdr *)pb_network_header(pb);
	
	printf("\n[802.1Q] priority:%x cfi:%x id:%x type:%x", vlanh->pcp, vlanh->cfi, vlanh->vid, vlanh->type);

	proto = ntohs(vlanh->type);

	pb->data += sizeof(*vlanh);
	pb_set_network_header(pb, (pb->data - pb->head));
	
	inet_proto_handler(pb);

	return 0;

hdr_error:
	DBGMSG("802.1q header error!\n");
	return -1;
}

static struct ether_proto vlan_proto = {
	.name 		= "8021Q",
	.proto 		=  ETH_P_8021Q,
	.handler 	= vlan_handler,
};

void __init vlan_init(void)
{
	ether_proto_register(&vlan_proto);
}

