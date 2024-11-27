# IoT Smart Home with Dual Monitoring Options

This project implements a smart home system that allows users to monitor and control their devices using **two monitoring options**:  
1. **Blynk Dashboard**  
2. **Telegram Bot**

## Features
- **Blynk Monitoring**: A user-friendly dashboard for real-time monitoring and control of IoT devices.  
- **Telegram Bot Monitoring**: Receive notifications and interact with your IoT devices directly from Telegram.  

## Requirements
### Hardware:
- ESP32 or ESP8266 microcontroller
- IoT devices (e.g., lights, fans, sensors)

### Software:
- [Blynk App](https://blynk.io/)
- Telegram app (with your bot configured)
- Arduino IDE with the necessary libraries installed

## Setup Guide
### Step 1: Hardware Connection
Connect the IoT devices to the microcontroller following the circuit diagram provided in the repository.

### Step 2: Blynk Configuration
1. Create a new project in the Blynk app and note the **auth token**.
2. Configure the WiFi credentials and Blynk auth token in the provided code.

### Step 3: Telegram Bot Setup
1. Create a new bot using [BotFather](https://core.telegram.org/bots#botfather) on Telegram and get the **bot token**.
2. Add the bot token and chat ID to the code.

### Step 4: Upload the Code
Use Arduino IDE to upload the appropriate code for either Blynk or Telegram monitoring.

### Step 5: Choose Monitoring Option
- For Blynk, use the Blynk app dashboard.  
- For Telegram, use the bot commands to interact with your smart home system.  

## Usage
- **Blynk**: Monitor device statuses and send commands via the dashboard.  
- **Telegram**: Receive real-time updates and send control commands through the bot.  

## Code Structure
- `smart-home-main.ino`: Code for Blynk monitoring.
- `esp-telegram-bot.ino`: Code for Telegram monitoring.
- `config.h`: Contains WiFi credentials, tokens, and other settings.

Cheers
