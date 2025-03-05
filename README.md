# ELVA-Embedded-Language-Model-and-Virtual-Assistant
An Embedded Virtual Assistant implemented on esp32 as an assignment for Embedded Systems Design and Analysis course at the University of Guilan, Department of Computer Engineering, presented in Fall 2024.


# System Overview
Elva is an embedded virtual assistant powered by a language model (basically a talking robot! :D). This project aims to develop a small embedded system capable of listening, processing, and responding to human speech, functioning as a smart virtual assistant. Various techniques and technologies were utilized to build this system, including C and Python programming, pre-trained large language models (LLM), speech-to-text (STT) and text-to-speech (TTS) models, as well as IoT and microcontroller integration.

# System Architecture and Workflow
This system consists of two major parts. The platform and the node. While the platform is responsible for processing user input and generating outputs through several stages, the node records user's voice, sends it to platform, receives generated output and plays it on speaker.

![Frame 38](https://github.com/user-attachments/assets/68417b15-864f-4fee-a9ac-36d57085bab7)

Here's how the system works.

1. The node records the user's voice.
2. The node saves the recorded voice as a .wav file on the SD card.
3. The node sends the input.wav file to the platform over the local network.
4. The platform receives and saves the input.wav file.
5. The platform converts the input.wav file to text using the Distil-Whisper STT model.
6. The platform processes the transcribed text using the LLaMA 3.2:1B LLM and saves the generated response.
7. The platform converts the response text to an .mp3 file using the Edge TTS model.
8. The platform sends the output.mp3 file back to the node over the local network.
9. The node receives and saves the output.mp3 file on the SD card.
10. The node plays the output.mp3 file through the speaker.

## The Platform
The platform consists of three parts. Large language model (LLM), speech-to-text (STT) and text-to-speech (TTS).
### Large Language Model
The LLM responsible for generating answers is the [LLaMA 3.2:1B](https://huggingface.co/meta-llama/Llama-3.2-1B) model, trained by Meta.

### Speech to Text Model
The STT model responsible for converting user's speech to text is the [Parler-TTS Mini](https://huggingface.co/parler-tts/parler-tts-mini-v1) model.

### Text to Speech Model
The TTS model responsible for converting generated answers to audio files is the [Edge TTS](https://huggingface.co/spaces/innoai/Edge-TTS-Text-to-Speech) model.

## The Node
The platform consists of four parts. The microcontroller (MCU), sound recorder, speaker and SD card.


### MCU
The microcontroller responsible for input, output and data transmissions is an ESP32 WROOM-32U.

### Sound Recorder
The sound recorder module which is used for recording user's voice is an I2S MEMS INMP441.

### Speaker
 an [Adafruit I2S Stereo Decoder - UDA1334A](https://www.adafruit.com/product/3678).


### sd card








# Demonstration
sth
