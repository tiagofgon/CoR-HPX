// #ifndef COR_MAILER_HPP
// #define COR_MAILER_HPP

// #include <thread>
// #include <map>
// #include <deque>
// #include <queue>
// #include <mutex>
// #include <condition_variable>

// #include "cor/utils/utils.hpp"

// #include "cor/system/macros.hpp"
// #include "cor/message.hpp"

// namespace cor {

// class Mailer
// {

// friend class ResourceManager;

// public:
//     explicit Mailer(std::string const& id, std::string const& app_group);
//     ~Mailer();

//     void SendMessage(idp_t idp, idp_t dest, Message& msg);
//     void SendMessage(idp_t idp, std::vector<idp_t> const& dests, Message& msg);

//     Message ReceiveMessage(idp_t idp);
//     Message ReceiveMessage(idp_t idp, idp_t source);

//     // Mailer(Mailer const&) = delete;
//     // Mailer& operator=(Mailer const&) = delete;
//     // Mailer(Mailer&&) = delete;
//     // Mailer& operator=(Mailer&&) = delete;

// protected:
//     // accessed by ResourceManager
//     void CreateMailbox(idp_t idp);
//     void DeleteMailbox(idp_t idp);

// private:
//     void HandleRead();
//     void HandleWrite();

//     void JoinGroup(std::string const& group);
//     void LeaveGroup(std::string const& group);

//     void JoinMessageGroup(idp_t idp);
//     void LeaveMessageGroup(idp_t idp);

//     std::string GetMessageGroup(idp_t idp);
//     idp_t GetIdpFromMessageGroup(std::string const& group);

//     enum class MsgType: std::int16_t
//     {
//         Message
//     };

//     constexpr typename std::underlying_type<MsgType>::type underlying_cast(MsgType t) const noexcept
//     {
//         return static_cast<typename std::underlying_type<MsgType>::type>(t);
//     }

//     std::string _app_group;

//     // resources mailboxes
//     std::map<idp_t, std::deque<Message>> _mailboxes;
//     std::map<idp_t, std::condition_variable> _rwq;
//     std::mutex _mtx;

//     struct Out
// 	{
//         bool ongoingWrite = false;
// 		std::queue<std::pair<std::vector<std::string>, std::string>> q;
// 	};
// 	utils::Monitor<Out> _out_queue;

// 	// Hold the currently outgoing message
// 	std::vector<std::string> _outgoing_groups;
// 	std::string _outgoing_msg;

// };

// }

// #endif
