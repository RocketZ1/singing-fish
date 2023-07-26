import mido
import requests
import time
import rtmidi
from concurrent.futures import ThreadPoolExecutor

# Singing Fish IP Address
BASS_IP = "192.168.254.162"
BASE_URL = f"http://{BASS_IP}/"

# Motor modes
MOUTH_MOTOR = "1"
TAIL_BODY_MOTOR = "2"

# Motor states
MOUTH_OFF = "0"
MOUTH_ON = "2"

TAIL_BODY_OFF = "0"
TAIL_BODY_HEAD_OUT = "1"
TAIL_BODY_TAIL_OUT = "2"


def send_motor_command_async(motor, mode):
    url = BASE_URL + f"{motor},{mode}"
    try:
        executor = ThreadPoolExecutor(max_workers=1)
        future = executor.submit(requests.get, url)
    except Exception:
        # Do nothing
        x = 1


def play_midi_file(file_path):
    mid = mido.MidiFile(file_path)

    mouth_open = False
    tail_out = False

    midi_out = rtmidi.MidiOut()
    available_ports = midi_out.get_ports()

    if available_ports:
        midi_out.open_port(0)  # Open the first available MIDI output port
    else:
        print("No MIDI output ports available. Exiting...")
        return

    start_time = time.time()
    prev_time = None  # Variable to store the previous timestamp

    for msg in mid.play():
        current_time = time.time() - start_time
        print(f"Current Timestamp: {current_time:.2f} seconds")

        midi_out.send_message(msg.bytes())  # Send the MIDI message to the output

        if prev_time is not None:
            # Calculate the duration between sending MIDI messages
            duration = current_time - prev_time
            time.sleep(max(0, msg.time - duration))  # Sleep for the remaining duration

        prev_time = current_time
        high_note = 64  # For the song "fnaf.mid", most notes above 64 are lyrics and below are instruments

        if msg.type == "note_on":
            if msg.note >= high_note:  # If higher, move mouth (Mouth)
                # Move the mouth motor only if it's not already open
                if not mouth_open:
                    send_motor_command_async(MOUTH_MOTOR, MOUTH_ON)
                    mouth_open = True
            else:  # If lower, move tail/body
                if not tail_out:
                    send_motor_command_async(TAIL_BODY_MOTOR, TAIL_BODY_TAIL_OUT)
                    tail_out = True

        elif msg.type == "note_off":
            if msg.note >= high_note:  # If higher, move mouth (Mouth)
                # Close the mouth motor only if it's open
                if mouth_open:
                    send_motor_command_async(MOUTH_MOTOR, MOUTH_OFF)
                    mouth_open = False
            else:  # If lower, move tail/body
                if tail_out:
                    send_motor_command_async(TAIL_BODY_MOTOR, TAIL_BODY_OFF)
                    tail_out = False

    midi_out.close_port()


if __name__ == "__main__":
    # Selected song/audio track to play. Must be in the same file directory as this python script
    midi_file_path = "fnaf.mid"
    play_midi_file(midi_file_path)
