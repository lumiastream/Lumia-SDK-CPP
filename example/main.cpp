#include "lumia.h"

void testSends(Lumia lumia)
{
	// Sending an alert event example
	lumia.sendAlert(LumiaAlertValues::TWITCH_FOLLOWER);

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
	ILumiaSendPack pack;

	pack = ILumiaSendPack();
	pack.type = LumiaCommandTypes::ALERT;
	pack.params.value = getTypeValue(LumiaAlertValues::TWITCH_FOLLOWER);
	lumia.send(pack);
}

int main()
{
	std::string token = "token";
	std::string appName = "lumia-test-sdk";
	Lumia lumia = Lumia();

	lumia.init([&]()
			   {
			std::cout << "inited" << std::endl;

			lumia.getInfo([&](json& res)
				{ std::cout << "Info: " << res.dump() << std::endl; });

			// set callback for all events ; not using an event library
			lumia.eventcb = [&](const std::string& eventName, std::variant<std::string, json>& data_)
			{
				std::cout << "Event Name: " << eventName << std::endl;

				if (eventName == "error")
				{
					auto* dataptr = std::get_if<json>(&data_);

					if (dataptr != nullptr)
					{
						auto& data = *dataptr;
						std::cout << "Error Event: " << data.dump();
					}
					else {
						std::string data = std::get<std::string>(data_);
						std::cout << "Event data: " << data;
					}
				}
				else
				{
					auto* dataptr = std::get_if<json>(&data_);

					if (dataptr != nullptr)
					{

						auto& data = *dataptr;

						std::cout << "Event data: " << data.dump();
						// here we give the context as we know it's an SDK Eent types
						switch (getTypeValueFromString<LumiaSdkEventTypes>("LumiaSdkEventTypes", data["type"]))
						{
						case LumiaSdkEventTypes::STATES:
							std::cout << "States have been updated: " << data.dump() << std::endl;
							break;

						case LumiaSdkEventTypes::COMMAND:
							std::cout << "A Chat Command is being triggered: " << data.dump() << std::endl;
							break;

						case LumiaSdkEventTypes::CHAT:
							std::cout << "New chat message: " << data.dump() << std::endl;
							break;

						case LumiaSdkEventTypes::ALERT:
							std::cout << "New alert: " << data.dump() << std::endl;
							break;
						}
					}
					else
					{
						std::string data = std::get<std::string>(data_);
						std::cout << "Event data: " << data;
					}
				}
			};

			// Uncomment this to test out sending events
			// testSends(lumia);
			// lumia.stop(); },
			   token, appName);

	return 0;
}
