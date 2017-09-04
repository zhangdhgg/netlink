#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>

#define NETLINK_TEST 21

#define MAX_PAYLOAD 1024 /* maximum payload size*/
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int main(int argc, char* argv[])
{
	/* Creat socket fd */
	sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
	/* Netlink message */
	memset(&msg, 0, sizeof(msg));
	/* Source address */
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid(); /* self pid */
	src_addr.nl_groups = 0; /* not in mcast groups */
	/* Bind */
	bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));

	/* Destination address */
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0; /* For Linux Kernel */
	dest_addr.nl_groups = 0; /* unicast */

	nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	/* Fill the netlink message header */
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid(); /* self pid */
	nlh->nlmsg_flags = 0;

	/* Fill in the netlink message payload */
	strcpy(NLMSG_DATA(nlh), "This is a netlink test !");

	/* Netlink body */
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;

	/* Destination address */
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	printf(" Sending message\n");
	sendmsg(sock_fd, &msg, 0);

	/* Read message */
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
	printf("Waiting message\n");
	recvmsg(sock_fd, &msg, 0);
	printf("Receive message payload: %s\n", NLMSG_DATA(nlh));

	/* Close netlink socket */
	close(sock_fd);
	return 0;
}
