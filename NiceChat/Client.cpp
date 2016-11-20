#include "stdafx.h"
#include "Client.h"
using namespace std;


std::mutex Client::mtx;


Client::Client()
{

}


Client::~Client()
{

}


Client* Client::GetInstance()
{
	static Client* client;
	if (client == NULL)
	{
		Client::mtx.lock();
		if (client == NULL)
		{
			client = new Client();
		}
		Client::mtx.unlock();
	}
	return client;
}


bool Client::TryLogin(string login, string pass, string *err_message)
{
	throw new runtime_error("unsupported exception");
}


bool Client::TryRegistrate(
	string name,
	string last_name,
	string login,
	string pass,
	string *err_message
)
{
	throw new runtime_error("unsupported exception");
}


void Client::LeaveChat()
{

}