import os
import json
import time
from flask import Flask, request

SAVE_DIR = "game_output"

os.makedirs(SAVE_DIR, exist_ok=True)

app = Flask(__name__)

def save_webhook(hook_id, body):
    timestamp = time.strftime("%Y-%m-%d_%H-%M-%S")
    safe_id = hook_id.replace("/", "_")

    filename = f"{timestamp}__{safe_id}.json"
    path = os.path.join(SAVE_DIR, filename)

    with open(path, "w", encoding="utf-8") as f:
        json.dump(body, f, indent=4)

    print(f"[Saved] {path}")


@app.route("/<path:hook_id>", methods=["POST"])
def receive(hook_id):
    try:
        data = request.json  # Unity JSON
    except:
        data = {"raw": request.get_data(as_text=True)}

    save_webhook(hook_id, data)
    return "OK", 200


if __name__ == "__main__":
    print("Listening on http://0.0.0.0:8000/")
    app.run(host="0.0.0.0", port=8000)
