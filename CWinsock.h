#include <ws2tcpip.h>

class cWinsocky
{
    public:
        cWinsocky ( VOID );

        VOID InitWinsock ( VOID );
        VOID Connect ( const char* pszServerUrl );
		VOID SendHttpGet ( const char* pszFileUrl, std::string &sDestBuffer );

        VOID Disconnect ( VOID );
        VOID CleanUp ( VOID );
    private:
        SOCKET m_ConnectedSocket;
        const char* m_pszConnectedUrl;

};

extern cWinsocky g_Winsock;