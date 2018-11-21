#include "DllMain.h"

cWinsocky g_Winsock;

cWinsocky::cWinsocky ( VOID )
{
    m_ConnectedSocket = INVALID_SOCKET;
}

VOID cWinsocky::InitWinsock ( VOID )
{
    WSADATA wsaData;
    
    if ( WSAStartup ( MAKEWORD ( 2, 2 ), &wsaData ) != 0 ) 
    {
#ifdef DEBUGMODE
		Base::Debug::LOG("WINSOCK ERROR: WSAStartup");
#endif
    }

}

VOID cWinsocky::Connect ( const char* pszServerUrl )
{
    m_pszConnectedUrl = pszServerUrl;

    addrinfo hints, *pAddrInfo;

    SecureZeroMemory ( &hints, sizeof ( hints ) );
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if ( getaddrinfo ( pszServerUrl, /*80*/XorStr<0x15,3,0x9AED279F>((const char*)"\x2D\x26"+0x9AED279F).s, &hints, &pAddrInfo ) != 0 ) 
    {
        CleanUp( );
#ifdef DEBUGMODE
		Base::Debug::LOG("WINSOCK ERROR: getaddrinfo");
#endif
    }

    if ( ( m_ConnectedSocket = socket ( pAddrInfo->ai_family, pAddrInfo->ai_socktype, pAddrInfo->ai_protocol ) ) == INVALID_SOCKET ) 
    {
        freeaddrinfo ( pAddrInfo );
        CleanUp( );
#ifdef DEBUGMODE
		Base::Debug::LOG("WINSOCK ERROR: socket");
#endif
    }

    if ( connect ( m_ConnectedSocket, pAddrInfo->ai_addr, ( INT ) pAddrInfo->ai_addrlen ) != 0 ) 
    {
        freeaddrinfo ( pAddrInfo );
        Disconnect( );
        CleanUp( );
#ifdef DEBUGMODE
		Base::Debug::LOG("WINSOCK ERROR: connect");
#endif
    }

    freeaddrinfo ( pAddrInfo );
}

VOID cWinsocky::SendHttpGet ( const char* pszFileUrl, std::string &sDestBuffer )
{
	std::string sMsg = /*GET /*/XorStr<0x57,6,0x59E137A6>("\x10\x1D\x0D\x7A\x74"+0x59E137A6).s;
    sMsg += pszFileUrl;
    sMsg += /* HTTP/1.0\r\nHost: */XorStr<0xBD,18,0x809D9B98>("\x9D\xF6\xEB\x94\x91\xED\xF2\xEA\xF5\xCB\xCD\x80\xA6\xB9\xBF\xF6\xED"+0x809D9B98).s; // 1.0 statt 1.1 wegen Chunked_transfer_encoding
    sMsg += m_pszConnectedUrl;
    sMsg += /*\r\n\r\n*/XorStr<0x79,5,0x2EE0F46F>("\x74\x70\x76\x76"+0x2EE0F46F).s;

    CHAR szRecvBuffer [ 512 ];

	if ( send ( m_ConnectedSocket, sMsg.data(), strlen ( sMsg.data() ), 0 ) == SOCKET_ERROR ) 
    {
        Disconnect( );
        WSACleanup( );
#ifdef DEBUGMODE
		Base::Debug::LOG("WINSOCK ERROR: send");
#endif
    }

    INT iBytesReceived = 0;
    do 
    {
        iBytesReceived = recv ( m_ConnectedSocket, szRecvBuffer, sizeof ( szRecvBuffer ), 0 );
        
		sDestBuffer.append ( szRecvBuffer, iBytesReceived );

    } while ( iBytesReceived > 0 );

    // httpheader entfernen
	for ( size_t i = 0; i < sDestBuffer.size(); i++ )
    {
        if ( sDestBuffer [ i ] == '\r' && sDestBuffer [ i + 1 ] == '\n' && sDestBuffer [ i + 2 ] == '\r' && sDestBuffer [ i + 3 ] == '\n' )
			sDestBuffer.erase( 0, i + 4 );
    }

}

VOID cWinsocky::Disconnect ( VOID )
{
    if ( m_ConnectedSocket != INVALID_SOCKET )
    {
        closesocket ( m_ConnectedSocket );
        m_ConnectedSocket = INVALID_SOCKET;
    }

}

VOID cWinsocky::CleanUp ( VOID )
{
    WSACleanup( );
}







