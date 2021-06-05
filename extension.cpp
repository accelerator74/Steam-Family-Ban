#include "extension.h"

SteamFamilyBan g_SteamFamilyBan;
SMEXT_LINK(&g_SteamFamilyBan);

SteamFamilyBan::SteamFamilyBan() :
	m_CallbackGSClientApprove(this, &SteamFamilyBan::OnGSClientApprove),
	m_CallbackValidateTicket(this, &SteamFamilyBan::OnValidateTicket)
{
	m_familySharingTable.init();
}

bool SteamFamilyBan::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	playerhelpers->AddClientListener(&g_SteamFamilyBan);
	return true;
}

void SteamFamilyBan::SDK_OnUnload()
{
	playerhelpers->RemoveClientListener(&g_SteamFamilyBan);
}

void SteamFamilyBan::OnClientPutInServer(int client)
{
	IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(client);

	if (pPlayer != NULL && !pPlayer->IsFakeClient())
	{
		if (m_familySharingTable.has(pPlayer->GetAuthString()))
		{
			gamehelpers->AddDelayedKick(client, pPlayer->GetUserId(), "Family Sharing users are not allowed to join this server");
		}
	}
}

void SteamFamilyBan::OnClientDisconnecting(int client)
{
	IGamePlayer *pPlayer = playerhelpers->GetGamePlayer(client);

	if (pPlayer != NULL && !pPlayer->IsFakeClient())
	{
		m_familySharingTable.removeIfExists(pPlayer->GetAuthString());
	}
}

void SteamFamilyBan::FamilyValidateClient(Account_t parent, Account_t child)
{
	if (parent > 0 && parent != child)
	{
		char auth[32];
		ke::SafeSprintf(auth, sizeof(auth), "STEAM_1:%d:%d", (child & 1), (child >> 1));

		m_familySharingTable.add(auth);

		char buffer[64];
		ke::SafeSprintf(buffer, sizeof(buffer), "banid %d %s\n", 30, auth);
		gamehelpers->ServerCommand(buffer);
	}
}

void SteamFamilyBan::OnGSClientApprove(GSClientApprove_t *pApprove)
{
	this->FamilyValidateClient(pApprove->m_OwnerSteamID.GetAccountID(), pApprove->m_SteamID.GetAccountID());
}

void SteamFamilyBan::OnValidateTicket(ValidateAuthTicketResponse_t *pTicket)
{
	this->FamilyValidateClient(pTicket->m_OwnerSteamID.GetAccountID(), pTicket->m_SteamID.GetAccountID());
}
