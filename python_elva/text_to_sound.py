import torch
from parler_tts import ParlerTTSForConditionalGeneration
from transformers import AutoTokenizer
import soundfile as sf

device = "cuda:0" if torch.cuda.is_available() else "cpu"

model = ParlerTTSForConditionalGeneration.from_pretrained("parler-tts/parler-tts-mini-expresso").to(device)
tokenizer = AutoTokenizer.from_pretrained("parler-tts/parler-tts-mini-expresso")

description = "Talia speaks at a slightly fast pace in a very confined sounding environment with clear audio quality."
input_ids = tokenizer(description, return_tensors="pt").input_ids.to(device)


def run_text_to_speech(text, output_address):
    prompt_input_ids = tokenizer(text, return_tensors="pt").input_ids.to(device)
    generation = model.generate(input_ids=input_ids, prompt_input_ids=prompt_input_ids)
    audio_arr = generation.cpu().numpy().squeeze()
    print(audio_arr)
    sf.write(output_address, audio_arr, model.config.sampling_rate)
