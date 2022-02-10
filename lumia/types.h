#pragma once
#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <variant>
#include <vector>
#include "json.hpp"
#include "typesLookup.h"
#include <stdexcept>

// for convenience
using json = nlohmann::json;

enum class LumiaCommandTypes : int
{
    ALERT = 1,
    MIDI = 2,
    OSC = 3,
    ARTNET = 4,
    RGB_COLOR = 5,
    HEX_COLOR = 6,
    CHAT_COMMAND = 7,
    TWITCH_POINTS = 8,
    TWITCH_EXTENSION = 9,
    TROVO_SPELLS = 10,
    STUDIO_SCENE = 11,
    STUDIO_ANIMATION = 12,
    STUDIO_THEME = 13,
    CHATBOT_MESSAGE = 14,
    TTS = 15,
    GAMESGLOW_ALERT = 201,
    GAMESGLOW_COMMAND = 202,
    GAMESGLOW_VARIABLE = 203,
    GAMESGLOW_VIRTUALLIGHT = 204,
};

enum class LumiaAlertValues
{
    TWITCH_FOLLOWER = 16,
    TWITCH_SUBSCRIBER = 17,
    TWITCH_BITS = 18,
    TWITCH_HOST = 19,
    TWITCH_RAID = 20,
    YOUTUBE_SUBSCRIBER = 21,
    YOUTUBE_SUPERCHAT = 22,
    YOUTUBE_SUPERSTICKER = 23,
    YOUTUBE_MEMBER = 24,
    FACEBOOK_FOLLOWER = 25,
    FACEBOOK_REACTION = 26,
    FACEBOOK_STAR = 27,
    FACEBOOK_SUPPORT = 28,
    FACEBOOK_SHARE = 29,
    FACEBOOK_FAN = 30,
    TROVO_FOLLOWER = 31,
    TROVO_SUBSCRIBER = 32,
    STREAMLABS_DONATION = 33,
    STREAMLABS_MERCH = 34,
    STREAMLABS_REDEMPTION = 35,
    STREAMLABS_PRIMEGIFT = 36,
    STREAMELEMENTS_DONATION = 37,
    OBS_SWITCHPROFILE = 38,
    OBS_SWITCHSCENE = 39,
    OBS_SWITCH_transition = 40,
    OBS_STREAMSTARTING = 41,
    OBS_STREAMSTOPPING = 42,
    SLOBS_SWITCHSCENE = 43,
    TREATSTREAM_TREAT = 44,
    PULSE_HEARTRATE = 45,
    PULSE_CALORIES = 46
};

enum class LumiaEventTypes
{
    STATES = 47,
    CHAT = 48,
    COMMAND = 49,
    TWITCH_POINTS = 50,
    TWITCH_EXTENSIONS = 51,
    TROVO_SPELL = 52,
    PULSE = 53,
    ALERT = 54,
    GAMESGLOW_ALERT = 55,
    GAMESGLOW_COMMAND = 56,
    GAMESGLOW_VIRTUALLIGHT = 57,
};

enum class Platforms
{
    TWITCH = 111,
    YOUTUBE = 112,
    FACEBOOK = 113,
    TROVO = 114,
    GLIMESH = 115
};

enum class LightBrands
{
    HUE = 116,
    NANOLEAF = 117,
    NANOLEAF2 = 118,
    LIFX = 119,
    TPLINK = 120,
    YEELIGHT = 121,
    COLOLIGHT = 122,
    TUYA = 123,
    SMARTLIFE = 124,
    WYZE = 125,
    WIZ = 126,
    HOMEASSISTANT = 127,
    GOVEE = 128,
    WLED = 129,
    MAGICHOME = 130,
    LOGITECH = 131,
    RAZER = 132,
    CORSAIR = 133,
    STEELSERIES = 134,
    OVERLAY = 135,
    ELGATO = 136
};

enum class EventOrigins
{
    LUMIA = 137,
    TWITCH = 138,
    YOUTUBE = 139,
    FACEBOOK = 140,
    GLIMESH = 141,
    TROVO = 142,
    STREAMLABS = 143,
    STREAMELEMENTS = 144,
    EXTRALIFE = 145,
    DONORDRIVE = 146,
    TILTIFY = 147,
    PATREON = 148,
    TIPEEESTREAM = 149,
    TREATSTREAM = 150,
    DISCORD = 151,
    OBS = 152,
    SLOBS = 153,
    PULSOID = 154,
    PAYPAL = 155
};

template <class T>
static const std::string getTypeValue(T value)
{
    return types_values[static_cast<int>(value)];
};

template <class T>
static const T getTypeValueFromString(const std::string &value_type, const std::string &value)
{

    if (value_type == "LumiaSDKCommandTypes")
    {
        return static_cast<T>(types_values_str_LumiaSDKCommandTypes[value]);
    }
    else if (value_type == "LumiaSDKAlertValues")
    {
        return static_cast<T>(types_values_str_LumiaSDKAlertValues[value]);
    }
    else if (value_type == "LumiaSdkEventTypes")
    {
        return static_cast<T>(types_values_str_LumiaSdkEventTypes[value]);
    }
    else if (value_type == "Platforms")
    {
        return static_cast<T>(types_values_str_Platforms[value]);
    }
    else if (value_type == "LightBrands")
    {
        return static_cast<T>(types_values_str_LightBrands[value]);
    }
    else if (value_type == "EventOrigins")
    {
        return static_cast<T>(types_values_str_EventOrigins[value]);
    }
    else
    {
        throw std::runtime_error("Invalid vlaue");
    }
};

struct RGB
{
    std::optional<int> r;
    std::optional<int> g;
    std::optional<int> b;
};

struct ExtraSetting
{
    std::optional<std::string> username;
    std::optional<int> bits;
};

struct ILumiaEventData
{
    std::optional<std::string> username;
    std::optional<std::string> command;
};

struct ILumiaEvent
{
    EventOrigins origin;
    LumiaEventTypes type;
    ILumiaEventData data;
};

struct ILumiaLight
{
    LightBrands type;
    std::variant<std::string, int> id;
};

struct LumiaPackParams
{

    std::variant<std::string, RGB> value;
    std::optional<std::vector<ILumiaLight>> lights;
    std::optional<bool> hold;      // Sets this command to default or not
    std::optional<bool> skipQueue; // Skips the queue and instantly turns to this color

    std::optional<Platforms> platform;

    // Used for TTS
    std::optional<std::string> voice;
    std::optional<int> volume;

    // Mainly used for RGB color and Hex color types
    std::optional<int> brightness;
    std::optional<int> transition;
    std::optional<int> duration;

    std::optional<ExtraSetting> extraSettings; // Mainly used to pass in variables for things like TTS or Chat bot
};
class ILumiaSendPack
{
public:
    LumiaCommandTypes type;
    LumiaPackParams params;
    LumiaPackParams gamesGlowName;
    LumiaPackParams glowId;

    json toJSONobj() const
    {
        json j;
        j["type"] = getTypeValue(type);

        j["params"] = {};

        if (gamesGlowName)
        {
            j["gamesGlowName"] = gamesGlowName;
        }
        if (glowId)
        {
            j["glowId"] = glowId;
        }

        auto *vpt = std::get_if<std::string>(&params.value);
        if (vpt != nullptr)
        {
            j["params"]["value"] = *vpt;
        }
        else
        {

            j["params"]["value"] = {};
            const RGB &rgb = std::get<RGB>(params.value);

            if (rgb.r != std::nullopt)
            {
                j["params"]["value"]["r"] = rgb.r.value();
            }
            if (rgb.g != std::nullopt)
            {
                j["params"]["value"]["g"] = rgb.g.value();
            }

            if (rgb.b != std::nullopt)
            {
                j["params"]["value"]["b"] = rgb.b.value();
            }
        }

        if (params.lights)
        {
            j["params"]["lights"] = json::array();
            int i = 0;
            for (const ILumiaLight &light : params.lights.value())
            {
                auto *idpt = std::get_if<int>(&light.id);
                if (idpt != nullptr)
                {
                    j["params"]["lights"][i++] = {
                        {"type", light.type},
                        {"id", std::get<int>(light.id)}};
                }
                else
                {
                    j["params"]["lights"][i++] = {
                        {"type", light.type},
                        {"id", std::get<std::string>(light.id)}};
                }
            }
        }

        if (params.hold)
        {
            j["params"]["hold"] = params.hold.value();
        }

        if (params.skipQueue)
        {
            j["params"]["skipQueue"] = params.skipQueue.value();
        }

        if (params.platform)
        {
            j["params"]["platform"] = getTypeValue(params.platform.value());
        }

        if (params.voice)
        {
            j["params"]["voice"] = params.voice.value();
        }

        if (params.volume)
        {
            j["params"]["volume"] = params.volume.value();
        }

        if (params.brightness)
        {
            j["params"]["brtightness"] = params.brightness.value();
        }

        if (params.transition)
        {
            j["params"]["transition"] = params.transition.value();
        }

        if (params.duration)
        {
            j["params"]["duration"] = params.duration.value();
        }

        if (params.duration)
        {
            j["params"]["duration"] = params.duration.value();
        }

        if (params.extraSettings)
        {
            j["params"]["extraSettings"] = {};
            if (params.extraSettings.value().username)
            {
                j["params"]["extraSettings"]["username"] = params.extraSettings.value().username.value();
            }

            if (params.extraSettings.value().bits)
            {
                j["params"]["extraSettings"]["bits"] = params.extraSettings.value().bits.value();
            }
        }

        return j;
    };

    std::string stringify()
    {
        return toJSONobj().dump();
    };
};
