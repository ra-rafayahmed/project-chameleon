import os
import json
import time
from flask import Flask, request

SAVE_DIR = "game_output"
os.makedirs(SAVE_DIR, exist_ok=True)

app = Flask(__name__)

def save_webhook(device_id, body):
    timestamp = time.strftime("%Y-%m-%d_%H-%M-%S")

    # Ensure safe folder names
    safe_id = "unknown_device" if not device_id else str(device_id).replace("/", "_")
    
    # Create per-device folder
    device_folder = os.path.join(SAVE_DIR, safe_id)
    os.makedirs(device_folder, exist_ok=True)

    # Save JSON file
    filename = f"{timestamp}.json"
    path = os.path.join(device_folder, filename)

    with open(path, "w", encoding="utf-8") as f:
        json.dump(body, f, indent=4)

    print(f"[Saved] {path}")


@app.route("/<path:hook_id>", methods=["POST"])
def receive(hook_id):
    data = {}

    # Try parsing as JSON first
    try:
        data = request.json or {}
    except:
        data = {"raw": request.get_data(as_text=True)}

    # Extract device ID from JSON payload (Unity must send it)
    device_id = data.get("deviceId") or "unknown_device"

    save_webhook(device_id, data)
    return "OK", 200


if __name__ == "__main__":
    print("Listening on http://0.0.0.0:8000/")
    app.run(host="0.0.0.0", port=8000)
