#pragma once

#include <iostream>
#include <string.h>
#include <memory>
#include "endpoint.h"
#include "types.h"
#include <functional>
#include <memory>

typedef std::function<void(json &)> callback;

class Lumia
{

public:
	typedef Lumia type;

	Lumia(std::optional<std::string> token = {}, std::optional<std::string> name = {}, std::optional<std::string> host = {})
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
	}

	void init(const std::function<void()> &cb, const std::optional<std::string> &token = {}, const std::optional<std::string> &name = {}, const std::optional<std::string> &host = {})
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
		stoped = false;
		while (!stoped)
		{

			endpoint_ = std::make_unique<Endpoint>();
			endpoint_->setCB(cb, std::bind(&type::close, this), std::bind(&type::message, this, _1), std::bind(&type::fail, this, _1));
			endpoint_->start(host_ + "/api?token=" + token_ + "&name=" + name_);
		}
	};

	void getInfo(const callback &cb = nullptr)
	{

		json o = {};
		o["method"] = "sdk:getinfo";
		sendWsMessage(o, cb);
	};

	void send(const ILumiaSdkSendPack &pack, const callback &cb = nullptr)
	{

		json o = pack.toJSONobj();
		o["lsorigin"] = "lumia-sdk";

		sendWsMessage(o, cb);
	}

	void sendAlert(const LumiaSDKAlertValues &alert, const callback &cb = nullptr)
	{

		ILumiaSdkSendPack pack;
		pack.type = LumiaSDKCommandTypes::ALERT;
		pack.params.value = getTypeValue<LumiaSDKAlertValues>(alert);
		send(pack, cb);
	}

	// Sends command
	void sendCommand(const std::string &command, const std::optional<bool> &default_ = std::nullopt, const std::optional<bool> &skipQueue = std::nullopt, const callback &cb = nullptr)
	{

		ILumiaSdkSendPack pack;
		pack.type = LumiaSDKCommandTypes::CHAT_COMMAND;
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
		const std::optional<std::vector<ILumiaSdkLight> > &lights = std::nullopt,
		const callback &cb = nullptr)
	{

		ILumiaSdkSendPack pack;
		pack.type = LumiaSDKCommandTypes::RGB_COLOR;
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

		ILumiaSdkSendPack pack;
		pack.type = LumiaSDKCommandTypes::RGB_COLOR;
		pack.params.brightness = brightness;
		pack.params.transition = transition;
		pack.params.skipQueue = skipQueue;
		send(pack, cb);
	};

	// Sends tts
	void sendTts(const std::string &text, const std::optional<int> volume = std::nullopt, const std::optional<std::string> &voice = std::nullopt, const callback &cb = nullptr)
	{
		ILumiaSdkSendPack pack;
		pack.type = LumiaSDKCommandTypes::TTS;
		pack.params.value = text;
		pack.params.volume = volume;
		pack.params.voice = voice;
		send(pack, cb);
	};

	// Sends Chatbot message
	void sendChatbot(const Platforms &platform, const std::string &text, const callback &cb = nullptr)
	{
		ILumiaSdkSendPack pack;
		pack.type = LumiaSDKCommandTypes::CHATBOT_MESSAGE;
		pack.params.value = text;
		pack.params.platform = platform;
		send(pack, cb);
	};

	void stop()
	{

		json o = {};
		o["method"] = "sdk:stop";
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
	std::string host_ = "ws://127.0.0.1:39231";
	std::unique_ptr<Endpoint> endpoint_;

	void sendWsMessage(json &o, callback cb = nullptr)
	{

		if (cb)
		{
			cbs[++eventCount] = cb;
			o["context"] = std::to_string(eventCount);
		}

		std::string data = o.dump();

		endpoint_->sendText(data);
	}

	void open()
	{
		std::variant<std::string, json> data;
		data.emplace<std::string>("");
		if (eventcb)
			eventcb("connected", data);
	}

	void close()
	{
		std::variant<std::string, json> data;
		data.emplace<std::string>("");
		if (eventcb)
			eventcb("closed", data);
	}

	void fail(const std::string &msg)
	{
		std::variant<std::string, json> data;
		data.emplace<std::string>(msg);
		if (eventcb)
			eventcb("failed", data);
	}

	void message(const std::string &payload)
	{

		try
		{
			json j = json::parse(payload);

			auto context = j.find("context");
			if (context != j.end())
			{
				auto cb = cbs.find(*context);
				if (cb != cbs.end())
				{
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
			std::variant<std::string, json> data;
			data.emplace<std::string>(e.what());
			if (eventcb)
				eventcb("error", data);
		}
	}

	int eventCount = 0;

	std::map<int, callback> cbs;

	bool stoped = true;
};