from pydub import AudioSegment


def convert_wav_to_mp3(wav_file_path, mp3_file_path, bitrate="192k"):
    try:
        # Load the WAV file
        audio = AudioSegment.from_file(wav_file_path, format="wav")

        # Export as MP3
        audio.export(mp3_file_path, format="mp3", bitrate=bitrate)
        print(f"Conversion successful! MP3 saved to: {mp3_file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")


# Example usage
wav_file = "input_2.wav"
mp3_file = "input_2.mp3"
convert_wav_to_mp3(wav_file, mp3_file)