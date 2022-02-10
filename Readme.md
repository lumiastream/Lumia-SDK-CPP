# The official Lumia Stream SDK for C++

This repository is for the Lumia Stream SDK releases and documentation.

Developers can use the Lumia Stream SDK to extend and control the Lumia Stream desktop app, enabling them to control smart lights, MIDI, DMX, OSC, OBS, TTS and so much more to create a custom and unique lighting experience.

## Table of Contents

<!-- toc -->

-   [The official Lumia Stream SDK for C++](#the-official-lumia-stream-sdk-for-c)
    -   [Table of Contents](#table-of-contents)
-   [Installation](#installation)
-   [Run the SDK](#run-the-sdk)
-   [Sample](#sample)
-   [Run a mock server](#run-a-mock-server)
-   [Events](#events)
    -   [States](#states)
    -   [Chat Command](#chat-command)
    -   [Chat](#chat)
    -   [Alert](#alert)
-   [Control](#control)
    -   [Get Settings](#get-settings)
    -   [Send Command](#send-command)
    -   [Send Color](#send-color)
    -   [Send Color to specific lights](#send-color-to-specific-lights)
    -   [Send Brightness](#send-brightness)
    -   [Send TTS](#send-tts)
    -   [Send Chat bot](#send-chat-bot)
    -   [Send Chat Command](#send-chat-command)
    -   [Send Chat](#send-chat)
    -   [Send Alert](#send-alert)
    -   [Resources](#resources)
    -   [Compiling from Terminal](#compiling-from-terminal)
    -   [Let's link](#lets-link)

<!-- tocstop -->

# Installation

`lumiastream's sdk` can easily be installed as a npm module:

```bash
npm i @lumiastream/sdk
```

# Run the SDK

We've also included an example for using the SDK.

To run the example head to [examples](https://github.com/lumiastream/Lumia-SDK-JS/examples) and you will see the `basic-example.js` file there.

Make sure you replace your token with the token that you will find in Lumia Stream's settings.
Here is how to [find your API token](https://dev.lumiastream.com/docs/get-a-token)

After you have your token and replaced it inside of the `basic-example.js`, you can now run:

```bash
npm install
node basic-example.js
```

This will first initialize the sdk to create the connection. Then it will listen in on events that are coming through. You can also test out sending events by uncommenting the testSends function.

# Sample

The following snippet shows a valid sdk example

```js
'use strict';

const { LumiaSdk, LumiaCommandTypes, LumiaAlertValues, LumiaEventTypes } = require('@lumiastream/sdk');

const token = 'insert-token-here';
const appName = 'lumia-test-sdk-js';

(async () => {
    sdk = new LumiaSdk();

    try {
        await sdk.init({ appName, token });

        sdk.on('event', (data) => {
            console.log('Event data: ', data);
            switch (data.type) {
                case LumiaEventTypes.CHAT_COMMANDS: {
                    console.log('Chat Command is being triggered', data);
                    break;
                }
                case LumiaEventTypes.CHAT_TWITCH: {
                    console.log('New chat message from twitch', data);
                    break;
                }
            }
        });

        // Sending a command
        await sdk.sendCommand({
            command: 'red',
        });

        // Sending a basic color
        await sdk.sendColor({
            color: { r: 255, g: 0, b: 255 },
            brightness: 60,
            duration: 1000,
        });
    } catch (err) {
        console.log('Init err: ', err);
    }
})();
```

# Run a mock server

Included in the SDK is a mock server that you can run to test things out without needing Lumia Stream

To run the server head to [examples](https://github.com/lumiastream/Lumia-SDK-JS/examples) and you will see the `test-server.js` file there.
Now run:

```bash
npm install
node test-server.js
```

This will send a few test events from the client as well as listen in on commands received from the client

# Events

Events are broadcasted by Lumia Stream to each connected client when an action occurs inside Lumia Stream.

These events range from the app state being changed, raw chat messages, chat commands, alerts and much more.

An event message will contain at least the following base fields:

-   `type` _LumiaEventTypes_: the type of event.
-   `origin` _EventOrigins (optional)_: where the event originated from. i.e: twitch for example
-   `data` _ILumiaEventStateBody | ILumiaEventChatCommandBody | ILumiaEventChatBody | ILumiaEventAlertBody | ILumiaEventStateBody (optional)_: the body of the event

Additional fields may be present in the event message depending on the event type.

### States

States indicate the current status the Lumia Stream is in

**Example Response:**

```json
{
    "origin": null,
    "type": "states",
    "data": { "on": 1, "streamMode": 1, "fuze": 0 }
}
```

---

### Chat Command

Lumia Stream has been prompted to trigger a command

**Example Response:**

```json
{
    "origin": "twitch",
    "type": "command",
    "data": { "username": "lumiastream", "command": "red" },
    "raw": { "username": "lumiastream", "command": "red" }
}
```

---

### Chat

A raw chat message that has been sent

**Example Response (From Twitch):**

```json
{
    "type": "chat",
    "data": {
        "channel": "#lumiastream",
        "message": "Wow",
        "username": "lumiastream",
        "userId": "163366031",
        "userColor": "#8A2BE2",
        "userColorRgb": "138,43,226",
        "platform": "twitch",
        "badgesRaw": "broadcaster/1,subscriber/0",
        "hasEmotes": false,
        "emotes": "",
        "rawMessageWithoutEmotes": "Wow",
        "emotesRaw": "",
        "user": {
            "badge-info": [],
            "badges": [],
            "client-nonce": "",
            "color": "#8A2BE2",
            "display-name": "lumiastream",
            "emotes": null,
            "first-msg": false,
            "flags": null,
            "id": "188ebc3d-e6e2-4b36-a125-0c4f0c0f54fd",
            "mod": false,
            "room-id": "163366031",
            "subscriber": true,
            "turbo": false,
            "user-id": "163366031",
            "user-type": null,
            "emotes-raw": null,
            "badge-info-raw": "subscriber/21",
            "badges-raw": "broadcaster/1,subscriber/0",
            "username": "lumiastream",
            "message-type": "chat",
            "isSelf": false,
            "vip": false,
            "tier3": true,
            "tier2": true,
            "tier1": true,
            "follower": false
        }
    }
}
```

---

### Alert

A raw alert has been sent

**Example Response (From Twitch Follow):**

```json
{
    "origin": "twitch",
    "type": "alert",
    "event": "twitch-follower",
    "data": { "userId": "12345", "username": "lumiastream" },
    "raw": { "userId": "12345", "username": "lumiastream" }
}
```

# Control

You can control Lumia Stream as well through the SDK in a variety of ways.

You will have the ability to send a command, an alert, a chat bot message, tts, a direct color, as well as even triggering certain lights.

### Get Settings

There are a few reasons why you may want to receive settings from Lumia Stream.
These settings will include all of the lights that are connected to Lumia, the current state of the app, whether the user is premium or not and more

**Example:**

```c++
const info = await sdk.getInfo();
```

---

### Send Command

The simplest way to use Lumia is first setting up a command inside of Lumia Stream and then recalling it with the SDK

**Example:**

```c++
await sdk.sendCommand({
 command: 'red',
});
```

---

### Send Color

Sending a color gives you the ability to set your lights to whatever color you choose along with the brightness and the duration. The duration is so that Lumia can revert back to defaul after the determined time

**Example:**

```c++
await sdk.sendColor({
 color: { r: 255, g: 0, b: 255 },
 brightness: 60,
 duration: 1000,
});
```

---

### Send Color to specific lights

Using the same sendColor method you can also choose which lights receive the color change

**Example:**

```c++
await sdk.sendColor({
 color: { r: 255, g: 0, b: 255 },
 brightness: 60,
 duration: 1000,
 lights: [{ type: 'hue', id: '10' }]
});
```

---

### Send Brightness

Sending brightness alone will keep all of your lights at their current state while only updating the brightness value

**Example:**

```c++
await sdk.sendBrightness({
 brightness: 100,
});
```

---

### Send TTS

Sending TTS messages will give you the ability to use Lumia's TTS by just caling the sendTts method

**Example:**

```c++
await sdk.sendTts({
 text: 'This SDK is the best',
});
```

---

### Send Chat bot

Sending a Chat bot messages will allow you to send messages to chat through Lumia's Chat bot

**Example:**

```c++
await sdk.sendChatbot({
 platform: 'twitch',
 text: 'This SDK is the best',
});
```

---

### Send Chat Command

Lumia Stream has been prompted to trigger a command

**Example Response:**

```json
{
    "origin": "twitch",
    "type": "command",
    "data": { "username": "lumiastream", "command": "red" },
    "raw": { "username": "lumiastream", "command": "red" }
}
```

---

### Send Chat

A raw chat message that has been sent

**Example Response (From Twitch):**

```json
{
    "type": "chat",
    "data": {
        "channel": "#lumiastream",
        "message": "Wow",
        "username": "lumiastream",
        "userId": "163366031",
        "userColor": "#8A2BE2",
        "userColorRgb": "138,43,226",
        "platform": "twitch",
        "badgesRaw": "broadcaster/1,subscriber/0",
        "hasEmotes": false,
        "emotes": "",
        "rawMessageWithoutEmotes": "Wow",
        "emotesRaw": "",
        "user": {
            "badge-info": [],
            "badges": [],
            "client-nonce": "",
            "color": "#8A2BE2",
            "display-name": "lumiastream",
            "emotes": null,
            "first-msg": false,
            "flags": null,
            "id": "188ebc3d-e6e2-4b36-a125-0c4f0c0f54fd",
            "mod": false,
            "room-id": "163366031",
            "subscriber": true,
            "turbo": false,
            "user-id": "163366031",
            "user-type": null,
            "emotes-raw": null,
            "badge-info-raw": "subscriber/21",
            "badges-raw": "broadcaster/1,subscriber/0",
            "username": "lumiastream",
            "message-type": "chat",
            "isSelf": false,
            "vip": false,
            "tier3": true,
            "tier2": true,
            "tier1": true,
            "follower": false
        }
    }
}
```

---

### Send Alert

Send a mock alert

**Example:**

```c++
await sdk.sendAlert({ alert: LumiaAlertValues.TWITCH_FOLLOWER });
```

---

## Resources

-   [Download the latest Lumia Stream SDK release](https://github.com/lumiastream/Lumia-SDK-JS/releases)
-   [Read the full API reference](https://dev.lumiastream.com)
-   [Run a mock server](https://github.com/lumiastream/Lumia-SDK-JS/examples)
-   [Browse some examples](https://github.com/lumiastream/Lumia-SDK-JS/examples)

## Compiling from Terminal

```bash
cd example
g++ -std=c++17 main.cpp -o test_sdk -I/path/to/Lumia-SDK-CPP/example/include
./test_sdk
```

## Let's link

Reach out to us on Discord to show off what you're working on, or to just lounge around and speak about ideas [Link](https://discord.gg/R8rCaKb)
