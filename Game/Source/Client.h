#pragma once

#include <functional>
#include <array>
#include <filesystem>
#include "Base.h"
#include "ClientConnectHistory.h"
#include <Rso/Core/Stream.h>
#include <Rso/Base/Base.h>
#include <Rso/Base/OptionStream.h>
#include <Rso/Net/Client.h>
#include <Rso/Mobile/Client.h>

namespace rso::game
{
	using namespace std;
	using namespace filesystem;
	using namespace core;
	using namespace base;
	using namespace net;
	using namespace mobile;

	class CClient : public INet
	{
		enum _ENode
		{
			Auth,
			Master,
			Server,
			Max,
			Null
		};

		struct _SClient
		{
			EProto CaProto = EProto::Null;
			_ENode CurNode = _ENode::Null; // UnLink, LinkFail ���� Client.CurNode != ���� �ݹ������ Node�� �ٸ��� ó�� ���� �ʱ� �����̰�,
										// �ٸ��ٴ� ���� �ܺο������� Disconnect �Ȱ��� �ƴϰ�, ���ο��� ���������� Close ������ �ǹ�.
			unique_ptr<COptionStream<CNamePort>> MasterNamePort;
			unique_ptr<COptionStream<SLoginInfo>> LastLoginInfo;
			CNamePort AuthNamePort;
			TUID UID = 0; // ������ Logout() ȣ���� ��쿡 �������� �ʴ� �ǵ��� UID==0 ���� ǥ���ϰ� ���� UnLink �ݹ鿡�� �� ���� �õ� ���� �ʵ��� ��.
			TID ID; // ���� �α��� �Ǵ� ����� UID�� ���� ID�� �α��� �� ��츸 �ӽ� ����ǰ� �׷��� ������ �α��� �����ϴ��� ID�� �������� ����.(���ʿ�) ���Ӽ��� �α��� �ÿ� ID����.
			TNick Nick;
			TUID SubUID = 0; // SubUID �� �� Ŭ���� �ܺο��� �̹� �����Ǳ� ������ UIDPair�� ��������. 
			TState State = 0;
			CStream Stream;
			TSessionCode SessionCode;
			bool Logon = false;
			CClientConnectHistory ConnectHistory;

			_SClient(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& AuthNamePort_, const TID& ID_, const TNick& Nick_, TUID SubUID_, TState State_, const CStream& Stream_) : // For Create
				CaProto(EProto::CaCreate),
				MasterNamePort(new COptionStream<CNamePort>(DataPath_ + L"Master_" + to_wstring(PeerNum_) + L".dat", true)),
				LastLoginInfo(new COptionStream<SLoginInfo>(DataPath_ + L"LastLoginInfo_" + to_wstring(PeerNum_) + L".dat", true)),
				AuthNamePort(AuthNamePort_),
				ID(ID_),
				Nick(Nick_),
				SubUID(SubUID_),
				State(State_),
				Stream(Stream_)
			{
				create_directories(DataPath_);
			}
			_SClient(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& AuthNamePort_, TUID SubUID_, const CStream& Stream_) : // For Login
				CaProto(EProto::CaLogin),
				MasterNamePort(new COptionStream<CNamePort>(DataPath_ + L"Master_" + to_wstring(PeerNum_) + L".dat", true)),
				LastLoginInfo(new COptionStream<SLoginInfo>(DataPath_ + L"LastLoginInfo_" + to_wstring(PeerNum_) + L".dat", true)),
				AuthNamePort(AuthNamePort_),
				UID((*LastLoginInfo)->UID),
				ID((*LastLoginInfo)->ID),
				SubUID(SubUID_),
				Stream(Stream_)
			{
				create_directories(DataPath_);
			}
			_SClient(const wstring& DataPath_, const CNamePort& AuthNamePort_, const TID& ID_, TUID SubUID_) : // For Check
				CaProto(EProto::CaCheck),
				AuthNamePort(AuthNamePort_),
				ID(ID_),
				SubUID(SubUID_)
			{
				create_directories(DataPath_);
			}
			inline SCaCreate GetCaCreate(void) const
			{
				return SCaCreate(ID, Nick, State);
			}
			inline SCaLogin GetCaLogin(void) const
			{
				return SCaLogin(UID, ID);
			}
			inline SCaCheck GetCaCheck(void) const
			{
				return SCaCheck(ID);
			}
			_ENode GetNodeToConnect(const TID& ID_)
			{
				//�����б⼺������    �ܺ�ID��ȿ  ID��ġ����  ó��
				//x                   x           .           �α��� ����
				//x                   o           .           Auth �� �α���
				//o                   x           .           Master�� �α���
				//o                   o           x           Auth �� �α���
				//o                   o           o           Master�� �α���

				if ((*LastLoginInfo)->UID == 0)
				{
					if (ID_.empty())
					{
						return _ENode::Null;
					}
					else
					{
						ID = ID_;
						return _ENode::Auth;
					}
				}
				else
				{
					if (ID_.empty() || (*LastLoginInfo)->ID == ID_)
					{
						return _ENode::Master;
					}
					else
					{
						UID = 0;
						ID = ID_;
						return _ENode::Auth;
					}
				}
			}
			inline bool IsCreate(void) const
			{
				return (CaProto == EProto::CaCreate);
			}
			inline bool IsCheck(void) const
			{
				return (CaProto == EProto::CaCheck);
			}
			inline bool IsValidAccount(void) const
			{
				return (UID > 0 || ID.size() > 0);
			}
			void SetNullAccount(void)
			{
				UID = 0;
				ID.clear();
			}
			void Clear(void)
			{
				MasterNamePort->Clear();
				LastLoginInfo->Clear();
			}
			void Login(const TNick& Nick_)
			{
				Nick = Nick_;
				Logon = true;
				*LastLoginInfo = SLoginInfo(UID, ID);
				LastLoginInfo->Save();
			}
			void ReleaseAccount(void)
			{
				LastLoginInfo->Clear();
			}
		};

	public:
		using TLinkFunc = function<void(const CKey& Key_, TUID UID_, const TNick& Nick_, const TFriends& Friends_)>;
		using TLinkFailFunc = function<void(TPeerCnt PeerNum_, EGameRet GameRet_)>;
		using TUnLinkFunc = function<void(const CKey& Key_, EGameRet GameRet_)>;
		using TErrorFunc = function<void(TPeerCnt PeerNum_, EGameRet GameRet_)>;
		using TCheckFunc = function<void(TUID UID_, CStream& Stream_)>;
		using TFriendAddedFunc = function<void(TPeerCnt PeerNum_, TUID UID_, const SFriend& Friend_)>;
		using TFriendRequestedFunc = function<void(TPeerCnt PeerNum_, TUID UID_, const TNick& Nick_)>;
		using TFriendAllowedFunc = function<void(TPeerCnt PeerNum_, TUID FriendUID_)>;
		using TFriendDenyedFunc = function<void(TPeerCnt PeerNum_, TUID FriendUID_)>;
		using TFriendBlockedFunc = function<void(TPeerCnt PeerNum_, TUID FriendUID_)>;
		using TFriendUnBlockedFunc = function<void(TPeerCnt PeerNum_, TUID FriendUID_)>;
		using TStateChangedFunc = function<void(TPeerCnt PeerNum_, TState State_)>;
		using TFriendStateChangedFunc = function<void(TPeerCnt PeerNum_, TUID FriendUID_, TState State_)>;
		using TMessageReceivedFunc = function<void(TPeerCnt PeerNum_, TUID FriendUID_, const TMessage& Message_)>;

		TLinkFunc LinkFunc;
		TLinkFailFunc LinkFailFunc;
		TUnLinkFunc UnLinkFunc;
		TRecvFunc RecvFunc;
		TErrorFunc ErrorFunc;
		TCheckFunc CheckFunc;
		TFriendAddedFunc FriendAddedFunc;
		TFriendRequestedFunc FriendRequestedFunc;
		TFriendAllowedFunc FriendAllowedFunc;
		TFriendDenyedFunc FriendDenyedFunc;
		TFriendBlockedFunc FriendBlockedFunc;
		TFriendUnBlockedFunc FriendUnBlockedFunc;
		TStateChangedFunc StateChangedFunc;
		TFriendStateChangedFunc FriendStateChangedFunc;
		TMessageReceivedFunc MessageReceivedFunc;

	private:
		using _TClients = CList<_SClient>;

		SVersion _Version;
		_TClients _Clients;
		net::CClient _NetA;
		net::CClient _NetM;
		net::CClient _NetS;

		EGameRet _NetRetToGameRet(ENetRet NetRet_);
		void _LoginClear(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _LoginFail(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _LoginFailAndCloseA(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _LoginFailAndCloseM(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _LoginFailAndCloseS(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _CheckFail(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _CheckFailAndClose(TPeerCnt PeerNum_, EGameRet GameRet_);
		void _Connect(TPeerCnt PeerNum_, _ENode Node_, const CNamePort& NamePort_);
		void _ConnectToUpper(TPeerCnt PeerNum_, _ENode Node_);
		bool _ConnectToLower(TPeerCnt PeerNum_, _ENode Node_, const CNamePort& NamePort_);
		void _LinkA(const CKey& Key_);
		void _LinkFailA(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkA(const CKey& Key_, ENetRet NetRet_);
		void _RecvA(const CKey& Key_, CStream& Stream_);
		void _RecvAcLogin(const CKey& Key_, CStream& Stream_);
		void _RecvAcLoginFail(const CKey& Key_, CStream& Stream_);
		void _RecvAcCheck(const CKey& Key_, CStream& Stream_);
		void _RecvAcCheckFail(const CKey& Key_, CStream& Stream_);
		void _LinkM(const CKey& Key_);
		void _LinkFailM(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkM(const CKey& Key_, ENetRet NetRet_);
		void _RecvM(const CKey& Key_, CStream& Stream_);
		void _RecvMcLogin(const CKey& Key_, CStream& Stream_);
		void _RecvMcLoginFail(const CKey& Key_, CStream& Stream_);
		void _LinkS(const CKey& Key_);
		void _LinkFailS(TPeerCnt PeerNum_, ENetRet NetRet_);
		void _UnLinkS(const CKey& Key_, ENetRet NetRet_);
		void _RecvS(const CKey& Key_, CStream& Stream_);
		void _RecvScLogin(const CKey& Key_, CStream& Stream_);
		void _RecvScLoginFail(const CKey& Key_, CStream& Stream_);
		void _RecvScError(const CKey& Key_, CStream& Stream_);
		void _RecvScAddFriendRequest(const CKey& Key_, CStream& Stream_);
		void _RecvScAddFriend(const CKey& Key_, CStream& Stream_);
		void _RecvScAllowFriend(const CKey& Key_, CStream& Stream_);
		void _RecvScDenyFriend(const CKey& Key_, CStream& Stream_);
		void _RecvScBlockFriend(const CKey& Key_, CStream& Stream_);
		void _RecvScUnBlockFriend(const CKey& Key_, CStream& Stream_);
		void _RecvScChangeState(const CKey& Key_, CStream& Stream_);
		void _RecvScFriendStateChanged(const CKey& Key_, CStream& Stream_);
		void _RecvScFriendStateChangedOffline(const CKey& Key_, CStream& Stream_);
		void _RecvScMessageReceived(const CKey& Key_, CStream& Stream_);
		void _RecvScUserProto(const CKey& Key_, CStream& Stream_);

	public:
		CClient(const SVersion& Version_);
		virtual ~CClient() {}
		bool IsLinked(TPeerCnt PeerNum_) const;
		inline void Close(TPeerCnt PeerNum_)
		{
			_NetS.Close(PeerNum_);
		}
		inline CNamePort GetNamePort(TPeerCnt PeerNum_) const
		{
			return _NetS.GetNamePort(PeerNum_);
		}
		inline bool Close(const CKey& Key_)
		{
			return _NetS.Close(Key_);
		}
		inline void CloseAll(void)
		{
			_NetS.CloseAll();
		}
		inline void WillClose(TPeerCnt PeerNum_, const milliseconds& WaitMilliseconds_)
		{
			_NetS.WillClose(PeerNum_, WaitMilliseconds_);
		}
		inline bool WillClose(const CKey& Key_, const milliseconds& WaitMilliseconds_)
		{
			return _NetS.WillClose(Key_, WaitMilliseconds_);
		}
		inline TPeerCnt GetPeerCnt(void) const
		{
			return _NetS.GetPeerCnt();
		}
		inline milliseconds Latency(TPeerCnt PeerNum_) const
		{
			return _NetS.Latency(PeerNum_);
		}
		void Proc(void);
		inline bool IsConnecting(TPeerCnt PeerNum_) const
		{
			return bool(_Clients.get(PeerNum_));
		}
		void Create(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& HubNamePort_, const TID& ID_, const TNick& Nick_, TUID SubUID_, TState State_, const CStream& Stream_);
		bool Login(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& HubNamePort_, const TID& ID_, TUID SubUID_, const CStream& Stream_);
	private:
		void _Logout(_TClients::iterator Client_);
	public:
		void Logout(TPeerCnt PeerNum_);
		void Logout(void);
		void Check(TPeerCnt PeerNum_, const wstring& DataPath_, const CNamePort& AuthNamePort_, const TID& ID_, TUID SubUID_);
		void ReleaseAccount(TPeerCnt PeerNum_); // ���� ����
		template<typename... _TCsProto>
		inline void Send(TPeerCnt PeerNum_, _TCsProto&&... Protos_)
		{
			_NetS.Send(PeerNum_, SHeader(EProto::CsUserProto), Protos_...);
		}
		template<typename... _TCsProto>
		inline void Send(const CKey& Key_, _TCsProto&&... Protos_)
		{
			_NetS.Send(Key_, SHeader(EProto::CsUserProto), Protos_...);
		}
		void AddFriend(TPeerCnt PeerNum_, const TNick& Nick_);
		void AllowFriend(TPeerCnt PeerNum_, TUID UID_);
		void DenyFriend(TPeerCnt PeerNum_, TUID UID_);
		void BlockFriend(TPeerCnt PeerNum_, TUID UID_);
		void UnBlockFriend(TPeerCnt PeerNum_, TUID UID_);
		void ChangeState(TPeerCnt PeerNum_, TState State_);
		void MessageSend(TPeerCnt PeerNum_, TUID ToUID_, const TMessage& Message_);
	};
}
