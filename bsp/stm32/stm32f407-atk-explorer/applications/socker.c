#include <stdio.h>
 #include <string.h>
 #include <rtthread.h>
 #include "lwip/sockets.h"
 #include "lwip/netdb.h"
 #include "lwip/sys.h"
 #define TCP_TEST_HOST "192.168.0.103"
 #define TCP_TEST_PORT ( 1212u )
 #define TEST_BUFSZ ( 1024u )
static const char * send_tcp_req_data = "Hi, I am from tcp client." ;
static void _tcp_test ( void )
{
	int ret , i ;
	char * recv_data ;
	struct hostent * host ;
	struct hostent *gethostbyname(const char *name);
	int sock = - 1, bytes_received ;
	struct sockaddr_in server_addr ;
	host=gethostbyname(TCP_TEST_HOST);
	if (! host )
	{

		rt_kprintf ( "gethostbyname failed!\r\n" ); 
		return ;
	} 
	else
	{ 
		rt_kprintf ( "gethostbyname success!\r\n" );
	}
	recv_data = calloc ( 1, TEST_BUFSZ);
	if ( recv_data == RT_NULL)
 
	{
 
		rt_kprintf ( "No memory\n" );
 
		return ;
 
	}
 
	/* K*0Z socket ¡§2??_ SOCKET_STREAM¡§ TCP AT , TLS 2?? */
 
	if (( sock = socket ( AF_INET, SOCK_STREAM, 0)) < 0)
	  
	{
 
		rt_kprintf ( "Socket error\n" );
 
		goto __exit ;
 
	}
 
	else
 
	{
 
		rt_kprintf ( "Socket pass\n" );
 
	}
    /* M?	FN¡¤F?,¡¤=?1¡®p */
 server_addr . sin_family = AF_INET;
 server_addr . sin_port = htons ( TCP_TEST_PORT);
 server_addr . sin_addr = *(( struct in_addr *) host -> h_addr );
 memset(&( server_addr . sin_zero ), 0, sizeof ( server_addr . sin_zero ));
   
 rt_kprintf ( "will connect...\n" );
 if ( connect ( sock , ( struct sockaddr *)& server_addr , sizeof ( struct
sockaddr )) < 0)
 {
 rt_kprintf ( "Connect fail!\n" );
 goto __exit ;
 }
 else
 {
 rt_kprintf ( "Connect pass!\n" );
 }
 /* F1?¡® socket F? */
 ret = send ( sock , send_tcp_req_data , strlen ( send_tcp_req_data ), 0);
 if ( ret <= 0)
 {
 rt_kprintf ( "send error,close the socket.\n" );
 goto __exit ;
 }
 else
 {
 rt_kprintf ( "send pass!\n" );
 }
   
 /* ?f??	y?,¡¤?¡§?+X¡ª??PE? */
 bytes_received = recv ( sock , recv_data , TEST_BUFSZ - 1, 0);
 if ( bytes_received <= 0)
 {
 rt_kprintf ( "received error,close the socket.\n" );
 goto __exit ;
 }
   
 rt_kprintf ( "recv data:\n" );
 for ( i = 0; i < bytes_received ; i ++)
 {
 rt_kprintf ( "%c" , recv_data );
 }
   
 __exit :
 if ( recv_data )
 rt_free ( recv_data );
   
 if ( sock >= 0)
 closesocket( sock );
 }
   
static void tcp_test ( void )
 {
 rt_thread_t tid ;
 tid = rt_thread_create ( "tcp" , _tcp_test , NULL, 4096 , 23, 5);
 if ( tid )
 rt_thread_startup ( tid );
 else
 {
 rt_kprintf ( "tcp thread create failed!\r\n" );
 }
 }
   
 #ifdef FINSH_USING_MSH
 #include <finsh.h>
 MSH_CMD_EXPORT( tcp_test , TCP function test );
 #endif /* FINSH_USING_MSH */