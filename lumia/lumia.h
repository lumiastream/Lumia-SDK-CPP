#pragma once

#include <iostream>
#include <string.h>
#include <memory>
#include "endpoint.h"
#include "types.h"
#include <functional>
#include <memory>
#include <chrono>
#include <thread>

typedef std::function<void(json &)> callback;

class Lumia
{

public:
	typedef Lumia type;

	Lumia(std::optional<std::string> token = {}, std::optional<std::string> name = {}, std::optional<std::string> host = {}, const std::optional<int> &port = {})
	{

		if (token)
		{
			token_ = token.value();
		}

		if (name)
		{
			name_ = name.value();
		}

		if (host)
		{
			host_ = host.value();
		}

		if (port)
		{
			port_ = port.value();
		}
	}

	void init(const std::function<void()> &cb, const std::optional<std::string> &token = {}, const std::optional<std::string> &name = {}, const std::optional<std::string> &host = {}, const std::optional<int> &port = {})
	{

		if (token)
		{
			token_ = token.value();
		}

		if (name)
		{
			name_ = name.value();
		}

		if (host)
		{
			host_ = host.value();
		}

		if (port)
		{
			port_ = port.value();
		}
		stoped = false;
		while (!stoped)
		{
			isConnected = false;
			endpoint_ = std::make_unique<Endpoint>();
			endpoint_->setCB(cb, std::bind(&type::close, this, _1, _2), std::bind(&type::message, this, _1), std::bind(&type::fail, this, _1, _2, _3));
			endpoint_->start("ws://" + host_ + ":" + port_ + "/api?token=" + token_ + "&name=" + name_);

			if (delay_)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			}
		}
	};

	void getInfo(const callback &cb = nullptr)
	{
		json o = {};
		o["method"] = "retrieve";
		o["retrieve"] = true;
		sendWsMessage(o, cb);
	};

	void send(const ILumiaSendPack &pack, const callback &cb = nullptr)
	{

		json o = pack.toJSONobj();
		o["lsorigin"] = "lumia-sdk";

		sendWsMessage(o, cb);
	}

	void sendAlert(const LumiaAlertValues &alert, const callback &cb = nullptr)
	{

		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::ALERT;
		pack.params.value = getTypeValue<LumiaAlertValues>(alert);
		send(pack, cb);
	}

	// Sends command
	void sendCommand(const std::string &command, const std::optional<bool> &default_ = std::nullopt, const std::optional<bool> &skipQueue = std::nullopt, const callback &cb = nullptr)
	{

		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::CHAT_COMMAND;
		pack.params.value = command;
		pack.params.hold = default_;
		pack.params.skipQueue = skipQueue;
		send(pack, cb);
	};

	// Sends a color pack
	void sendColor(
		const RGB &color,
		const std::optional<int> &brightness = std::nullopt, // 0-100
		const std::optional<int> &duration = std::nullopt,	 // In milliseconds
		const std::optional<int> &transition = std::nullopt, // In milliseconds
		const std::optional<bool> &default_ = std::nullopt,
		const std::optional<bool> &skipQueue = std::nullopt,
		const std::optional<std::vector<ILumiaLight> > &lights = std::nullopt,
		const callback &cb = nullptr)
	{

		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::RGB_COLOR;
		pack.params.brightness = brightness;
		pack.params.duration = duration;
		pack.params.transition = transition;
		pack.params.hold = default_;
		pack.params.skipQueue = skipQueue;
		pack.params.lights = lights;
		send(pack, cb);
	};

	// Sends brightness only
	void sendBrightness(const int &brightness, const std::optional<int> &transition = std::nullopt, const std::optional<bool> &skipQueue = std::nullopt, const callback &cb = nullptr)
	{

		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::RGB_COLOR;
		pack.params.brightness = brightness;
		pack.params.transition = transition;
		pack.params.skipQueue = skipQueue;
		send(pack, cb);
	};

	// Sends tts
	void sendTts(const std::string &text, const std::optional<int> volume = std::nullopt, const std::optional<std::string> &voice = std::nullopt, const callback &cb = nullptr)
	{
		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::TTS;
		pack.params.value = text;
		pack.params.volume = volume;
		pack.params.voice = voice;
		send(pack, cb);
	};

	// Sends Chatbot message
	void sendChatbot(const Platforms &platform, const std::string &text, const callback &cb = nullptr)
	{
		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::CHATBOT_MESSAGE;
		pack.params.value = text;
		pack.params.platform = platform;
		send(pack, cb);
	};

	// Games glow functions

	void getGamesGlowSettings(const callback &cb = nullptr)
	{
		json o = {};
		o["method"] = "gamesGlowSettings";
		o["gamesGlowKey"] = name_;
		sendWsMessage(o, cb);
	};

	void sendGamesGlowAlert(const std::string &gamesGlowKey, const std::string &value, const callback &cb = nullptr)
	{
		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::GAMESGLOW_ALERT;
		pack.gamesGlowId = name_;
		pack.gamesGlowKey = gamesGlowKey;
		pack.params.value = value;
		send(pack, cb);
	};

	void sendGamesGlowCommand(const std::string &gamesGlowKey, const std::string &value, const callback &cb = nullptr)
	{
		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::GAMESGLOW_COMMAND;
		pack.gamesGlowId = name_;
		pack.gamesGlowKey = gamesGlowKey;
		pack.params.value = value;
		send(pack, cb);
	};

	void sendGamesGlowVariableUpdate(const std::string &gamesGlowKey, const std::string &value, const callback &cb = nullptr)
	{
		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::GAMESGLOW_VARIABLE;
		pack.gamesGlowId = name_;
		pack.gamesGlowKey = gamesGlowKey;
		pack.params.value = value;
		send(pack, cb);
	};

	void sendGamesGlowVirtualLightsChange(const std::string &gamesGlowKey, const std::string &value, const callback &cb = nullptr)
	{
		ILumiaSendPack pack;
		pack.type = LumiaCommandTypes::GAMESGLOW_VIRTUALLIGHT;
		pack.gamesGlowId = name_;
		pack.gamesGlowKey = gamesGlowKey;
		pack.params.value = value;
		send(pack, cb);
	};

	void stop()
	{

		json o = {};
		o["method"] = "stop";
		sendWsMessage(o, [&](json j) {});
		stoped = true;
		this->endpoint_->stop();
		std::variant<std::string, json> data;
		data.emplace<std::string>("");
		if (eventcb)
			eventcb("closed", data);
	};

	std::function<void(const std::string &, std::variant<std::string, json> &)> eventcb;

private:
	std::string token_;
	std::string name_;
	std::string host_ = "127.0.0.1";
	std::string port_ = 39231;
	std::unique_ptr<Endpoint> endpoint_;

	void sendWsMessage(json &o, callback cb = nullptr)
	{

		if (cb)
		{
			cbs[std::to_string(++eventCount)] = cb;
			o["context"] = std::to_string(eventCount);
		}

		std::string data = o.dump();

		endpoint_->sendText(data);
	}

	void open()
	{
		isConnected = true;
		std::variant<std::string, json> data;
		data.emplace<std::string>("");
		if (eventcb)
			eventcb("connected", data);
	}

	void close(int code, int coder)
	{

		std::variant<std::string, json> data;
		data.emplace<std::string>("");
		if (eventcb)
			eventcb("closed", data);
	}

	void fail(const std::string &msg, int code, int coder)
	{
		if (msg == "Invalid HTTP status.")
		{
			stoped = true;
		}
		else
		{
			if (coder == 1006 || code == 1006)
			{
				delay_ = true;
			}
		}

		std::variant<std::string, json> data;
		data.emplace<std::string>(msg);
		if (eventcb)
			eventcb("failed", data);
	}

	void message(const std::string &payload)
	{

		std::cout << payload;

		try
		{
			json j = json::parse(payload);

			auto context = j.find("context");
			if (context != j.end())
			{
				auto cb = cbs.find(*context);
				if (cb != cbs.end())
				{
					// j.erase("context");
					// j.erase("event");
					cb->second(j);
				};
				return;
			}
			std::variant<std::string, json> data;
			data.emplace<json>(j);
			if (eventcb)
				eventcb("event", data);
		}
		catch (std::exception &e)
		{
			std::cout << e.what();
			std::variant<std::string, json> data;
			data.emplace<std::string>(e.what());
			if (eventcb)
				eventcb("error", data);
		}
	}

	int eventCount = 0;

	std::map<std::string, callback> cbs;

	bool stoped = true;

	bool delay_ = false;

	bool isConnected = false;
};
