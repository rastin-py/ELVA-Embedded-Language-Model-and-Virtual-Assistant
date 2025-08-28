# ELVA-Embedded-Language-Model-and-Virtual-Assistant
An rmbedded virtual assistant implemented on esp32 as an assignment for Embedded Systems Design and Analysis course at the University of Guilan, Department of Computer Engineering, presented in Fall 2024.


# System Overview
Elva is an embedded virtual assistant powered by a language model (basically a talking robot!). This project aims to develop a small embedded system capable of listening, processing, and responding to human speech, functioning as a smart virtual assistant. Various techniques and technologies were utilized to build this system, including C and Python programming, large language models (LLM), speech-to-text (STT) and text-to-speech (TTS) models, as well as IoT and microcontroller integration.


# System Architecture and Workflow
This system consists of two main parts. The platform and the node. While the platform is responsible for processing user input and generating outputs through several stages, the node records user's voice, sends it to platform, receives generated output and plays it on speaker.

![Frame 38](https://github.com/user-attachments/assets/704e7ea3-8f7f-46b4-81cc-8361a426b699)

# System Overview

## How the System Works

1. The node records the user's voice.
2. The node saves the recorded voice as a `.wav` file on the SD card.
3. The node sends the `input.wav` file to the platform over the local network.
4. The platform receives and saves the `input.wav` file.
5. The platform converts the `input.wav` file to text using the Distil-Whisper STT model.
6. The platform processes the transcribed text using the LLaMA 3.2:1B LLM and generates a response.
7. The platform converts the response text to an `.mp3` file using the Edge TTS model.
8. The platform sends the `output.mp3` file back to the node over the local network.
9. The node receives and saves the `output.mp3` file on the SD card.
10. The node plays the `output.mp3` file through the speaker using the I2S stereo decoder.

## The Platform

The platform consists of three main components: the large language model (LLM), the speech-to-text (STT) model, and the text-to-speech (TTS) model.

### Large Language Model (LLM)
The LLM responsible for generating responses is the [LLaMA 3.2:1B](https://huggingface.co/meta-llama/Llama-3.2-1B) model, developed by Meta.

### Speech-to-Text (STT) Model
The STT model responsible for converting the user's speech to text is the [Distil-Whisper](https://huggingface.co/openai/whisper-small) model.

### Text-to-Speech (TTS) Model
The TTS model responsible for converting generated responses to audio files is the [Edge TTS](https://huggingface.co/spaces/innoai/Edge-TTS-Text-to-Speech) model.

## The Node

The node consists of four main components: the microcontroller (MCU), the I2S microphone, the I2S stereo decoder, and the SD card.

### Microcontroller (MCU)
The microcontroller responsible for handling input, output, and data transmissions is the ESP32 WROOM-32U.

![ESP32 WROOM-32U](https://github.com/user-attachments/assets/8a7afb32-e783-4733-aa07-3c4196477602)

### I2S Microphone
The microphone module used to capture the user's voice is the I2S MEMS INMP441.

![I2S MEMS INMP441](https://github.com/user-attachments/assets/5907bd88-85b9-4ac5-9add-234825dd9830)

### Stereo Decoder
The stereo decoder module used to decode the `output.mp3` file for the speaker is the Adafruit I2S Stereo Decoder - UDA1334A.

![Adafruit I2S Stereo Decoder - UDA1334A](https://github.com/user-attachments/assets/051d9a0c-c388-406a-a58f-0f2c20ce686d)

### SD Card
The SD card module used for storing input and output files is a 6-pin micro SD card reader.

![SD](https://github.com/user-attachments/assets/83326721-7cf7-466b-97e6-b79493da2bb5)

# Demonstration

## Video
In this video I ask elva to introduce herself and tell me about her capabilities.
You'll notice some delay in her response, this is because I don't have a powerful GPU.

https://github.com/user-attachments/assets/3f235ed9-3f47-4f73-8132-50f54075d9ae

## Picture
Right now she's just a bunch of wires. One day, I might build a proper case and body for her :D

![IMG_20250304_232344](https://github.com/user-attachments/assets/22589383-e088-44a5-a1cf-9f9475034f49)

# Future Developement Ideas
* 📷 Integrate a Camera: Capture images and send them to the LLM for visual perception, enabling Elva to "see."
* 😄 Add an LCD Display: Use an LCD to show Elva’s emotions, expressions, or interactive feedback visually.
* 🖨️ 3D-Print a Custom Case: Design physical case to enhance appearance, portability, and protection.
* 🧠 Fine-Tune LLM for Specialized Tasks: Customize Elva’s language model for tasks like database querying, RAG, or domain-specific interactions.
* 🚀 Upgrade to a More Powerful LLM: Run more advanced models like Gemma once I have access to better GPU.
