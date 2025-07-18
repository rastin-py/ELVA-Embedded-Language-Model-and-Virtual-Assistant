import asyncio
import edge_tts

VOICE = 'en-US-AriaNeural'


async def main(text, output_address):
    communicate = edge_tts.Communicate(text, VOICE, rate="+10%")
    await communicate.save(output_address)


def run_text_to_speech(text, output_address):
    asyncio.run(main(text, output_address))
