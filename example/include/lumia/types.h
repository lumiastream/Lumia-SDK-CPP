#pragma once
#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <variant>
#include <vector>
#include "json.hpp"
#include "typesLookup.h"

// for convenience
using json = nlohmann::json;




enum class LumiaSDKCommandTypes : int {
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
    TTS = 15
};


enum class LumiaSDKAlertValues {
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




enum class LumiaSdkEventTypes {
    CHAT = 47,
    CHAT_TWITCH = 48,
    CHAT_FACEBOOK = 49,
    CHAT_YOUTUBE = 50,
    CHAT_GLIMESH = 51,
    CHAT_TROVO = 52,
    CHAT_COMMANDS = 53,
    CHAT_COMMANDS_TWITCH = 54,
    CHAT_COMMANDS_FACEBOOK = 55,
    CHAT_COMMANDS_YOUTUBE = 56,
    CHAT_COMMANDS_GLIMESH = 57,
    CHAT_COMMANDS_TROVO = 58,
    TIMEOFUSE = 59,
    LIVE = 60,
    TWITCH_POINTS = 61,
    TWITCH_EXTENSIONS = 62,
    PULSE_RATE_MAX = 63,
    PULSE_RATE_MIN = 64,
    PULSE_CALORIES_MAX = 65,
    PULSE_CALORIES_MIN = 66,
    ALERTS = 67,
    ALERTS_TWITCH = 68,
    ALERTS_TWITCH_FOLLOWERS = 69,
    ALERTS_TWITCH_SUBSCRIBERS = 70,
    ALERTS_TWITCH_BITS = 71,
    ALERTS_TWITCH_HOSTS = 72,
    ALERTS_TWITCH_RAIDS = 73,
    ALERTS_YOUTUBE = 74,
    ALERTS_YOUTUBE_SUBSCRIBERS = 75,
    ALERTS_YOUTUBE_MEMBERS = 76,
    ALERTS_YOUTUBE_SUPERCHATS = 77,
    ALERTS_YOUTUBE_SUPERSTICKERS = 78,
    ALERTS_FACEBOOK = 79,
    ALERTS_FACEBOOK_FOLLOWERS = 80,
    ALERTS_FACEBOOK_REACTIONS = 81,
    ALERTS_FACEBOOK_STARS = 82,
    ALERTS_FACEBOOK_SUPPORTS = 83,
    ALERTS_FACEBOOK_SHARES = 84,
    ALERTS_FACEBOOK_FANS = 85,
    ALERTS_GLIMESH = 86,
    ALERTS_GLIMESH_FOLLOWERS = 87,
    ALERTS_GLIMESH_SUBSCRIBERS = 88,
    ALERTS_STREAMLABS = 89,
    ALERTS_STREAMLABS_DONATIONS = 90,
    ALERTS_STREAMLABS_CHARITY = 91,
    ALERTS_STREAMLABS_MERCH = 92,
    ALERTS_STREAMLABS_REDEMPTIONS = 93,
    ALERTS_STREAMLABS_PRIMEGIFTS = 94,
    ALERTS_STREAMELEMENTS = 95,
    ALERTS_STREAMELEMENTS_DONATIONS = 96,
    ALERTS_STREAMELEMENTS_MERCH = 97,
    ALERTS_STREAMELEMENTS_REDEMPTIONS = 98,
    ALERTS_EXTRALIFE_DONATIONS = 99,
    ALERTS_DONORDRIVE_DONATIONS = 100,
    ALERTS_PAYPAL_PAYMENTCOMPLETE = 101,
    ALERTS_PAYPAL_PAYMENTDENIED = 102,
    ALERTS_TIPEEESTREAM_DONATIONS = 103,
    ALERTS_TILTIFY_CAMPAIGNDONATIONS = 104,
    ALERTS_PATREON_CAMPAIGNPLEDGES = 105,
    ALERTS_TREATSTREAM_TREATS = 106,
    ALERTS_OBS = 107,
    ALERTS_SLOBS = 108,
    ALERTS_PULSE_RATE = 109,
    ALERTS_PULSE_CALORIES = 110
};

enum class Platforms {
    TWITCH = 111,
    YOUTUBE = 112,
    FACEBOOK = 113,
    TROVO = 114,
    GLIMESH = 115
};


enum class LightBrands {
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


enum class EventOrigins {
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
static const std::string getTypeValue(T value) {
    return types_values[static_cast<int>(value)];
};


template <class T>
static const T getTypeValueFromString(std::string value) {
    return static_cast<T>(types_values_str[value]);  
  
};

struct RGB {
    std::optional<int> r;
    std::optional<int> g;
    std::optional<int> b;
};


struct ExtraSetting {
    std::optional <std::string> username;
    std::optional <int> bits;
};


struct ILumiaSdkEventData {
    std::optional <std::string> username;
    std::optional <std::string> command;
};


struct ILumiaSdkEvent {
    EventOrigins origin;
    LumiaSdkEventTypes type;
    ILumiaSdkEventData data;
};



struct ILumiaSdkLight {
    LightBrands type;
    std::variant <std::string, int> id;
};


struct LumiaSDKPackParams {

    std::variant <std::string, RGB>  value;
    std::optional <std::vector<ILumiaSdkLight>> lights;
    std::optional <bool> hold ; // Sets this command to default or not
    std::optional <bool> skipQueue ; // Skips the queue and instantly turns to this color

    std::optional <Platforms> platform ;

    // Used for TTS
    std::optional <std::string> voice ;
    std::optional <int> volume ;

    // Mainly used for RGB color and Hex color types
    std::optional <int> brightness ;
    std::optional <int> transition;
    std::optional <int> duration ;

    std::optional <ExtraSetting> extraSettings; // Mainly used to pass in variables for things like TTS or Chat bot
    
};
class ILumiaSdkSendPack {
public:
    LumiaSDKCommandTypes type;
    LumiaSDKPackParams params;

    json toJSONobj() const {
        json j;
        j["type"] = getTypeValue(type);
        
        j["params"] = {};

        auto* vpt = std::get_if<std::string>(&params.value);
        if (vpt != nullptr) {
            j["params"]["value"] = *vpt;
        }
        else {

            j["params"]["value"] = {};
            const RGB& rgb = std::get<RGB>(params.value);

            if (rgb.r != std::nullopt) {
                j["params"]["value"]["r"] = rgb.r.value();
            } 
            if (rgb.g != std::nullopt) {
                j["params"]["value"]["g"] = rgb.g.value();
            }

            if (rgb.b != std::nullopt) {
                j["params"]["value"]["b"] = rgb.b.value();
            }
            
        }

        if (params.lights) {
            j["params"]["lights"] = json::array();
            int i = 0;
            for (const ILumiaSdkLight& light: params.lights.value()) {
                auto* idpt = std::get_if<int>(&light.id);
                if (idpt != nullptr) {
                    j["params"]["lights"][i++] = {
                        { "type" , light.type },
                        { "id" , std::get<int>(light.id) }
                    };
                }
                else {
                    j["params"]["lights"][i++] = {
                        { "type" , light.type },
                        { "id" , std::get<std::string>(light.id) }
                    };
                }
                


            }
            
        }

        if (params.hold) {
            j["params"]["hold"] = params.hold.value();
        }

        if (params.skipQueue) {
            j["params"]["skipQueue"] = params.skipQueue.value();
        }

        if (params.platform) {
            j["params"]["platform"] = getTypeValue(params.platform.value());
        }

        if (params.voice) {
            j["params"]["voice"] = params.voice.value();
        }

        if (params.volume) {
            j["params"]["volume"] = params.volume.value();
        }

        if (params.brightness) {
            j["params"]["brtightness"] = params.brightness.value();
        }

        if (params.transition) {
            j["params"]["transition"] = params.transition.value();
        }

        if (params.duration) {
            j["params"]["duration"] = params.duration.value();
        }

        if (params.duration) {
            j["params"]["duration"] = params.duration.value();
        }

        if (params.extraSettings) {
            j["params"]["extraSettings"] = {};
            if (params.extraSettings.value().username) {
                j["params"]["extraSettings"]["username"] = params.extraSettings.value().username.value();
            }
            
            if (params.extraSettings.value().bits) {
                j["params"]["extraSettings"]["bits"] = params.extraSettings.value().bits.value();
            }
            
        }


        return j;


    };

    std::string stringify() {
        return toJSONobj().dump();
    };
};








