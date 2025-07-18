import socket
import time
from AI import run_ai
from sound_to_text import run_sound_to_text
from edge_text_to_speech import run_text_to_speech

# ESP32 IP and port
ESP32_IP = "192.168.247.244"
ESP32_PORT = 12345

OUTPUT_FILE = "output.mp3"
INPUT_FILE = "input.wav"


def send_file():
    print("sending")
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((ESP32_IP, ESP32_PORT))
        print("Connected to ESP32.")
        with open(OUTPUT_FILE, "rb") as f:
            while (chunk := f.read(512)):
                sock.sendall(chunk)
        print("File sent successfully.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        sock.close()


def receive_file():
    print("receiving")
    try:
        # Create a socket and connect to the ESP32
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((ESP32_IP, ESP32_PORT))
        print("Connected to ESP32.")

        # Open file to save received data
        with open(INPUT_FILE, "wb") as f:
            while True:
                data = sock.recv(512)
                if not data:
                    break
                f.write(data)
        print(f"File received and saved as {INPUT_FILE}")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        sock.close()
        f.close()


def process_input(input_file_name, output_file_name):
    INPUT = run_sound_to_text(input_file_name)
    print("user says: " + INPUT)
    response = run_ai(INPUT)
    print("elva says:" + response)
    response  = response.replace('*', ' ')
    try:
        run_text_to_speech(response, output_file_name)
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    while True:
        receive_file()
        process_input('input.wav', 'output.mp3')
        send_file()
        time.sleep(5)
