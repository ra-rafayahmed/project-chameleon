import time
import threading
import signal
import sys

MY_PHONE = "923104017484"
SESSION_DB = "sync_probe.sqlite3"

rtts = []
pending_sends = {}

from neonize.client import NewClient
from neonize.events import event
from neonize.utils import build_jid

client = NewClient(SESSION_DB)


# -----------------------------
#   DELIVERY RECEIPTS
# -----------------------------
@client.event
def on_receipt(client, ev):
    if not hasattr(ev, "receipt"):
        return

    if ev.receipt.type != "delivered":
        return

    ids = []

    if hasattr(ev, "message_ids") and ev.message_ids:
        ids = ev.message_ids
    elif hasattr(ev, "ids") and ev.ids:
        ids = ev.ids
    elif hasattr(ev, "id") and ev.id:
        ids = [ev.id]

    for msg_id in ids:
        if msg_id in pending_sends:
            send_t = pending_sends[msg_id]
            rtt = time.time() - send_t

            status = "SCREEN ON" if rtt <= 1.0 else "SCREEN OFF"
            print(f"✓ Delivered | RTT={rtt:.3f}s | {status}")

            rtts.append(rtt)
            del pending_sends[msg_id]


# -----------------------------
#   PROBE SENDING
# -----------------------------
def probe_thread():
    target = build_jid(MY_PHONE)

    print("\n=== STARTING PROBES (NO SYNC MODE) ===\n")

    for i in range(1, 31):
        msg = client.send_message(target, "")
        msg_id = msg.id if hasattr(msg, "id") else str(msg)

        pending_sends[msg_id] = time.time()
        print(f"Probe {i}/30 sent (id={msg_id})")

        time.sleep(1)

    print("\nFinished sending probes!\n")


# -----------------------------
#   SIGNAL HANDLER
# -----------------------------
def signal_handler(sig, frame):
    print("\nStopping client...")
    try:
        client.disconnect()
    except:
        pass
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)


# -----------------------------
#   MAIN
# -----------------------------
print("Connecting WITHOUT SYNC...")

try:
    # Newer Neonize versions:
    client.connect(do_sync=False)
except:
    # Older Neonize fallback:
    client.connect(no_sync=True)

threading.Thread(target=probe_thread, daemon=True).start()

while True:
    time.sleep(1)
