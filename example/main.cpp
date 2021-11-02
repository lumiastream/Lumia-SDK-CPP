#include "lumia/lumia.h"

int main()
{

	std::string token = "token";
	std::string appName = "lumia-test-sdk";
	Lumia lumia = Lumia();
	ILumiaSdkSendPack pack;
	lumia.init([&]()
			   {
				   std::cout << "inited" << std::endl;

				   // set callback for all events ; not using an event library
				   lumia.eventcb = [&](const std::string &eventName, std::variant<std::string, json> &data_)
				   {
					   std::cout << "Event Name: " << eventName << std::endl;

					   auto *dataptr = std::get_if<json>(&data_);

					   if (dataptr != nullptr)
					   {

						   auto &data = *dataptr;

						   std::cout << "Event data: " << data.dump();

						   switch (getTypeValueFromString<LumiaSdkEventTypes>(data["type"]))
						   {
						   case LumiaSdkEventTypes::CHAT_COMMANDS:
							   std::cout << "Chat Command is being triggered: " << data.dump() << std::endl;
							   break;

						   case LumiaSdkEventTypes::CHAT_TWITCH:
						   {
							   std::cout << "New chat message from twitch: " << data.dump() << std::endl;
							   break;
						   }
						   }
					   }
					   else
					   {
						   std::string data = std::get<std::string>(data_);
						   std::cout << "Event data: " << data;
					   }
				   };

				   // Sending an alert event example
				   lumia.sendAlert(LumiaSDKAlertValues::TWITCH_FOLLOWER);

				   // Sending a command; with a callback to get the result for this call
				   lumia.sendCommand("red", {}, {}, [&](json &res)
									 { std::cout << res.dump() << std::endl; });

				   lumia.sendColor({255, 0, 255}, 60, 1000);

				   // Sending a brightness
				   lumia.sendBrightness(100);

				   // Sending a TTS message
				   lumia.sendTts("This SDK is the best");

				   // Sending a Chat bot message
				   lumia.sendChatbot(Platforms::TWITCH, "This SDK is the best");

				   // Sending a raw event example
				   pack = ILumiaSdkSendPack();
				   pack.type = LumiaSDKCommandTypes::ALERT;
				   pack.params.value = getTypeValue(LumiaSDKAlertValues::TWITCH_FOLLOWER);
				   lumia.send(pack);

				   //lumia.stop();
			   },
			   token, appName, host);

	return 0;
}