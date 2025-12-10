import os
import json
import glob
import shutil
from supabase import create_client, Client
from dotenv import load_dotenv

# Load Supabase credentials
load_dotenv("supabase.env")
SUPABASE_URL = os.getenv("SUPABASE_URL")
SUPABASE_KEY = os.getenv("SUPABASE_SERVICE_ROLE_KEY")

supabase: Client = create_client(SUPABASE_URL, SUPABASE_KEY)

# Paths
PROFILE_DIR = "output_profiles"
STORIES_DIR = "output_stories"
GAME_DIR = "game_output"
WHATSAPP_DIR = "device-activity-tracker/whatsapp_output"

# -------------------------------
# Utility: get or create unified user
# -------------------------------
def get_or_create_user(username=None, phone=None, device_id=None):
    """
    Returns user_id for a given username/phone/device_id.
    Reuses existing user if any of these exist, otherwise inserts a new user.
    """
    filters = []
    if phone:
        filters.append(("phone", phone))
    if username:
        filters.append(("username", username))
    if device_id:
        filters.append(("device_id", device_id))

    # Check existing
    for field, value in filters:
        resp = supabase.table("users").select("*").filter(field, "eq", value).limit(1).execute()
        data = resp.data
        if data and len(data) > 0:
            return data[0]["user_id"]

    # Insert new
    record = {"username": username, "phone": phone, "device_id": device_id}
    res = supabase.table("users").insert(record).execute()
    return res.data[0]["user_id"]

# -------------------------------
# Offload Instagram profiles
# -------------------------------
def process_profiles():
    print("➡ Processing Profiles...")
    for path in glob.glob(f"{PROFILE_DIR}/*_profile.json"):
        try:
            with open(path, "r", encoding="utf-8") as f:
                profile = json.load(f)

            username = profile.get("username")
            if not username:
                print(f"❌ Missing username — skipping {path}")
                continue

            user_id = get_or_create_user(username=username)
            # Insert profile
            profile_record = {
                "user_id": user_id,
                "username": username,
                "bio": profile.get("bio"),
                "posts_count": int(profile.get("posts_count", 0)),
                "followers_count": int(profile.get("followers_count", 0)),
                "following_count": int(profile.get("following_count", 0)),
                "raw": profile
            }
            supabase.table("instagram_profiles").insert(profile_record).execute()
            print(f"✅ Profile uploaded: {username}")
            os.remove(path)

        except Exception as e:
            print(f"❌ DB insert error for {path}: {e}")

# -------------------------------
# Offload Instagram stories
# -------------------------------
def process_stories():
    print("➡ Processing Stories...")
    for user_folder in os.listdir(STORIES_DIR):
        user_path = os.path.join(STORIES_DIR, user_folder)
        if not os.path.isdir(user_path):
            continue

        try:
            user_id = get_or_create_user(username=user_folder)
            for i, file_name in enumerate(os.listdir(user_path)):
                local_path = os.path.join(user_path, file_name)
                if not os.path.isfile(local_path):
                    continue

                # For simplicity we store local path and filename in DB
                story_record = {
                    "user_id": user_id,
                    "username": user_folder,
                    "story_index": i + 1,
                    "img_url": None,
                    "local_path": local_path,
                    "caption": f"{user_folder}'s story",
                    "raw": {"filename": file_name}
                }
                supabase.table("instagram_stories").insert(story_record).execute()
            # Remove folder after upload
            shutil.rmtree(user_path)
            print(f"✅ Stories uploaded: {user_folder}")

        except Exception as e:
            print(f"❌ Story upload error: {e}")

# -------------------------------
# Offload Game/device data
# -------------------------------
def process_game_data():
    print("➡ Processing Game Output...")
    for device_folder in os.listdir(GAME_DIR):
        device_path = os.path.join(GAME_DIR, device_folder)
        if not os.path.isdir(device_path):
            continue

        for json_file in glob.glob(f"{device_path}/*.json"):
            try:
                with open(json_file, "r", encoding="utf-8") as f:
                    game_data = json.load(f)

                device_id = game_data.get("deviceId")
                user_id = get_or_create_user(device_id=device_id)

                record = {
                    "user_id": user_id,
                    "device_id": device_id,
                    "gps_lat": game_data.get("gpsLatitude"),
                    "gps_lng": game_data.get("gpsLongitude"),
                    "ip": game_data.get("ipAddress"),
                    "timezone": game_data.get("timeZone"),
                    "system_language": game_data.get("systemLanguage"),
                    "device_region": game_data.get("deviceRegion"),
                    "keyboard_layout": game_data.get("keyboardLayout"),
                    "ping": game_data.get("pingEstimate"),
                    "raw": game_data
                }
                supabase.table("game_devices").insert(record).execute()
                print(f"✅ Game data uploaded: {os.path.basename(json_file)}")
                os.remove(json_file)

            except Exception as e:
                print(f"❌ Game data upload error for {json_file}: {e}")

        try:
            shutil.rmtree(device_path)
        except Exception as e:
            print(f"❌ Could not remove folder {device_path}: {e}")

# -------------------------------
# Offload WhatsApp events
# -------------------------------
def process_whatsapp_data():
    print("➡ Processing WhatsApp Output...")
    for file in glob.glob(f"{WHATSAPP_DIR}/*.json"):
        try:
            with open(file, "r", encoding="utf-8") as f:
                events = json.load(f)

            for ev in events:
                phone = None
                devices = ev.get("devices", [])
                if devices and len(devices) > 0:
                    phone = devices[0].get("jid")

                user_id = get_or_create_user(phone=phone) if phone else None

                record = {
                    "user_id": user_id,
                    "phone_number": phone,
                    "event_time": ev.get("time"),
                    "presence": ev.get("presence"),
                    "rtt": devices[0].get("rtt") if devices else None,
                    "device_count": ev.get("deviceCount"),
                    "devices": devices,
                    "raw": ev
                }
                supabase.table("whatsapp_events").insert(record).execute()
            os.remove(file)
            print(f"✅ WhatsApp data uploaded: {os.path.basename(file)}")

        except Exception as e:
            print(f"❌ WhatsApp DB err: {e}")

# -------------------------------
# Main
# -------------------------------
def main():
    print("🚀 Offloader started...")
    process_profiles()
    process_stories()
    process_game_data()
    process_whatsapp_data()
    print("✅ Offloading completed.")

if __name__ == "__main__":
    main()
