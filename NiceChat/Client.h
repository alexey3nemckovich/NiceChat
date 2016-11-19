#pragma once
#include <mutex>
#include <string>
using namespace std;


class Client
{
private:
	Client();
	~Client();
	std::mutex mtx;
public:
	Client* GetClient();
	bool TryLogin(string login, string pass, string *err_message);
	bool TryRegistrate(
		string login,
		string first_name,
		string second_name,
		string pass,
		string *err_message
	);
	//void ConnectTo();
	void LeaveChat();
};