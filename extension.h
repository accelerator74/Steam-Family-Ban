#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

/**
 * @file extension.h
 * @brief Sample extension code header.
 */

#include "smsdk_ext.h"
#include "steam_gameserver.h"
#include "am-hashset.h"
#include <sm_stringhashmap.h>

typedef uint32_t Account_t;

/**
 * @brief Sample implementation of the SDK Extension.
 * Note: Uncomment one of the pre-defined virtual functions in order to use it.
 */
class SteamFamilyBan : public SDKExtension, public IClientListener
{
public:
	SteamFamilyBan();

public:
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
	virtual void SDK_OnUnload();
	virtual void OnClientPutInServer(int client);
	virtual void OnClientDisconnecting(int client);

public:
	void FamilyValidateClient(Account_t parent, Account_t child);

private:
	STEAM_GAMESERVER_CALLBACK(SteamFamilyBan, OnGSClientApprove, GSClientApprove_t, m_CallbackGSClientApprove);
	STEAM_GAMESERVER_CALLBACK(SteamFamilyBan, OnValidateTicket, ValidateAuthTicketResponse_t, m_CallbackValidateTicket);

private:
	ke::HashSet<std::string, detail::StringHashMapPolicy> m_familySharingTable;
};

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
