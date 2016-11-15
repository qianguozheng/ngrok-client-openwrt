#include "config.h"
#include <string>

#if WIN32
#include <windows.h>
#else
#include <arpa/inet.h>
#include <errno.h>
typedef long long __int64;
#endif
#include "sendmsg.h"
#include<stdlib.h>
using namespace std;



char *rand_str(char *str,const int len)
{
    int i;
    for(i=0;i<len;++i)
        str[i]='A'+rand()%26;
    str[++i]='\0';
    return str;
}



int SendReqTunnel(int sock,ssl_context *ssl,char *ReqId,const char *protocol,const char * HostName,const char * Subdomain,int RemotePort,char *authtoken, int LocalPort)
{
    char guid[20]={0};
    rand_str(guid,5);
    char str[1024];
    memset(str,0,1024);
    memcpy(ReqId,guid,strlen(guid));//copy
    sprintf(str,"{\"Type\":\"ReqTunnel\",\"Payload\":{\"Protocol\":\"%s\",\"ReqId\":\"%s\",\"Hostname\": \"%s\",\"Subdomain\":\"%s\",\"HttpAuth\":\"\",\"RemotePort\":%d,\"authtoken\":\"%s\", \"LocalPort\":%d}}",protocol,guid,HostName,Subdomain,RemotePort,authtoken,LocalPort);
    return sendpack(sock,ssl,str,1);
}



int loadargs( int argc, char **argv ,list<TunnelInfo*>*tunnellist,char *s_name,int * s_port,char * authtoken,string *ClientId)
{
	if ( argc > 1 )
	{
		char	jsonstr[1024];
		char	temp[255];
		int	pos	= 0;
		char *argvstr;
		int	xpos;
		int	run = 1;
		for ( int i = 1; i < argc; i++ )
		{
			argvstr = argv[i];
			memset( jsonstr, 0, 1024 );
			memset( temp, 0, 255 );
			pos = strpos( argvstr, '[' );
			if ( pos == -1 )
			{
				printf( "argv error:%s", argvstr );
			}else  {
				if ( strncmp( argvstr, "-SER", 4 ) == 0 )
				{
					run = 1;
					while ( run )
					{
						memset( jsonstr, 0, 1024 );
						xpos = strpos( argvstr + pos + 1, ',' );
						if ( xpos == -1 )
						{
							xpos = strpos( argvstr + pos + 1, ']' );
							memcpy( jsonstr, argvstr + pos + 1, xpos );
							run = 0;
						}else  {
							memcpy( jsonstr, argvstr + pos + 1, xpos );
						}
						getvalue(jsonstr,"Shost",s_name);
						if(getvalue(jsonstr,"Sport",temp)==0)
                        {
                            *s_port = atoi(temp);
						}
						getvalue(jsonstr,"Atoken",authtoken);

						if(getvalue(jsonstr,"Cid",temp)==0)
                        {
                            *ClientId = string( temp );
						}

						pos = pos + xpos + 1;
					}
				}

				if ( strncmp( argvstr, "-AddTun", 7 ) == 0 )
				{
					run = 1;

					TunnelInfo *tunnelinfo = (TunnelInfo *) malloc( sizeof(TunnelInfo) );
					memset( tunnelinfo, 0, sizeof(TunnelInfo) );


					while ( run )
					{
						memset( jsonstr, 0, 1024 );
						xpos = strpos( argvstr + pos + 1, ',' );
						if ( xpos == -1 )
						{
							xpos = strpos( argvstr + pos + 1, ']' );
							memcpy( jsonstr, argvstr + pos + 1, xpos );
							run = 0;
						}else  {
							memcpy( jsonstr, argvstr + pos + 1, xpos );
						}


                        getvalue(jsonstr,"Lhost",tunnelinfo->localhost);
                        getvalue(jsonstr,"Type",tunnelinfo->protocol);
                        memset( temp, 0, strlen( temp ) );
                        if(getvalue(jsonstr,"Lport",temp)==0)
                        {
                            tunnelinfo->localport = atoi( temp );
                        }
                        memset( temp,0,strlen(temp));
                        if(getvalue(jsonstr,"Rport",temp)==0)
                        {
                            tunnelinfo->remoteport = atoi( temp );
                        }
                        getvalue(jsonstr,"Sdname",tunnelinfo->subdomain);
                        getvalue(jsonstr,"Hostname",tunnelinfo->hostname);
						pos = pos + xpos + 1;
					}

					(*tunnellist).push_back(tunnelinfo);
				}
			}
		}
	}else  {
		echo( "use " );
        echo("%s",argv[0]);
		echo( " -SER[Shost:ngrokd.ngrok.com,Sport:443,Atoken:xxxxxxx] -AddTun[Type:tcp,Lhost:127.0.0.1,Lport:80,Rport:50199]" );
		echo( "\r\n" );
		exit( 1 );
	}
	return 0;
}


char *get_iface_mac(const char *ifname)
{
	int r, s;
	struct ifreq ifr;
	char *hwaddr, mac[18];

	//printf("ifname=%s\n", ifname);
	
	strcpy(ifr.ifr_name, ifname);

	s = socket(PF_INET, SOCK_DGRAM, 0);
	if (-1 == s) {
		printf("get_iface_mac socket: %s", strerror(errno));
		return NULL;
	}

	r = ioctl(s, SIOCGIFHWADDR, &ifr);
	if (r == -1) {
		printf("get_iface_mac ioctl(SIOCGIFHWADDR): %s", strerror(errno));
		close(s);
		return NULL;
	}

	hwaddr = ifr.ifr_hwaddr.sa_data;
	close(s);
	
	#if 1
	snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X", 
			hwaddr[0] & 0xFF,
			hwaddr[1] & 0xFF,
			hwaddr[2] & 0xFF,
			hwaddr[3] & 0xFF,
			hwaddr[4] & 0xFF,
			hwaddr[5] & 0xFF
		);
    #endif
	
	
	return strdup(mac);
}

int execute_cmd(const char *uci_cmd, char *out, size_t osize)
{
	FILE *fp = NULL;
	int ret = -1;
	*out = 0;
	
	fp = popen(uci_cmd, "r");
	if (!fp) {
		printf("uci cmd %s error!", uci_cmd);
		return -1;
	}

	fgets(out, osize-1, fp);
	if (0 != out[0]) {  // not empty
		//out[osize-1] = 0;
		out[strlen(out)-1] = '\0';
		ret = 0;
	}

	pclose(fp);
	return ret;
}
