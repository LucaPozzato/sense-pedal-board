from flask import Flask, render_template, request, redirect, url_for, abort, jsonify
import os
import time
import logging
import sqlite3
import json
import atexit
import signal
import sys
import socket

app = Flask(__name__)
# logging.getLogger('werkzeug').setLevel(logging.ERROR)

def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # This IP doesn't need to be reachable; it's used to get the local IP address
        s.connect(("10.255.255.255", 1))
        ip = s.getsockname()[0]
    except Exception:
        ip = "127.0.0.1"
    finally:
        s.close()
    return ip

print("\033[2J\033[H\nIP Address:" + get_local_ip() + "\n\n")

REQUEST_INTERVAL = 2
last_request_time = time.time()

DB_FILE = 'state.db'

# --- Helper functions for the SQLite file-based store ---
def init_db():
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS state (
                    key TEXT PRIMARY KEY,
                    value TEXT
                )''')
    conn.commit()
    conn.close()

def get_state(key, default=None):
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute("SELECT value FROM state WHERE key = ?", (key,))
    row = c.fetchone()
    conn.close()
    if row:
        return json.loads(row[0])
    else:
        return default

def set_state(key, value):
    conn = sqlite3.connect(DB_FILE)
    c = conn.cursor()
    c.execute("INSERT OR REPLACE INTO state (key, value) VALUES (?, ?)",
              (key, json.dumps(value)))
    conn.commit()
    conn.close()

# --- Initialize the DB and default state if not present ---
init_db()
if get_state("pedal_array") is None:
    set_state("pedal_array", [0, 1, 3])
if get_state("curr_pedal") is None:
    set_state("curr_pedal", 0)
if get_state("pedal_value") is None:
    set_state("pedal_value", 0)

# Send the initial MIDI command using the current state.
pedal_array = get_state("pedal_array")
curr_pedal = get_state("curr_pedal")
if len(pedal_array) > 0:
    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')

# --- Flask routes ---
@app.get("/")
def index():
    return render_template("index.html", title='index')

@app.get("/next_pedal")
def next_pedal():
    global last_request_time
    current_time = time.time()
    elapsed_time = current_time - last_request_time
    if elapsed_time > REQUEST_INTERVAL:
        last_request_time = current_time
        # Read shared state from SQLite
        pedal_array = get_state("pedal_array", [])
        curr_pedal = get_state("curr_pedal", 0)
        
        # Send MIDI command for the current pedal
        os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')
        
        # Update the current pedal index
        new_curr_pedal = (curr_pedal + 1) % len(pedal_array)
        set_state("curr_pedal", new_curr_pedal)
        
        time.sleep(0.1)
        os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[new_curr_pedal]) + ' 60')
    return jsonify({"status": "success"})

@app.get("/pedal_state")
def pedal_state():
    pedal_array = get_state("pedal_array", [])
    curr_pedal = get_state("curr_pedal", 0)
    if len(pedal_array) == 0:
        return jsonify({"active_pedal": 0})
    return jsonify({"active_pedal": pedal_array[curr_pedal]})

@app.get("/pedal_value")
def get_pedal_value():
    pedal_value = get_state("pedal_value", 0)
    return jsonify({"pedal_value": pedal_value})

@app.post("/pedal_value")
def update_pedal_value():
    r_json = request.json
    value = r_json['pedal value']

    if int(value) == 0:
        scaled_value = 10
    else:
        scaled_value = int(value) / 9 * 127

    set_state("pedal_value", int(scaled_value))
    os.system('sendmidi dev "IAC Driver Bus 1" cc ' + str(scaled_value) + ' ' + str(scaled_value))
    return redirect(url_for('index'))

@app.post("/add_pedal")
def add_pedal():
    data = request.get_json()
    if not data:
        return jsonify({"status": "error", "message": "Invalid JSON"}), 400
    
    pedal_number = data.get("pedalNumber")
    if pedal_number is None:
        return jsonify({"status": "error", "message": "Pedal number is missing"}), 400

    current_pedal_array = get_state("pedal_array", [])
    if pedal_number in current_pedal_array:
        return jsonify({"status": "error", "message": f"Pedal {pedal_number} already exists"})
    
    if len(current_pedal_array) == 0:
        os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_number) + ' 60')
        set_state("curr_pedal", 0)

    current_pedal_array.append(pedal_number)
    set_state("pedal_array", current_pedal_array)
    print(f"Pedal {pedal_number} added")
    return jsonify({"status": "success", "message": f"Pedal {pedal_number} added successfully"})

@app.get("/clear_pedal_array")
def clear_array():
    pedal_array = get_state("pedal_array", [])
    curr_pedal = get_state("curr_pedal", 0)
    
    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')
    set_state("pedal_array", [])
    # set_state("curr_pedal", 0)
    
    # os.system('sendmidi dev "IAC Driver Bus 1" on 0 60')
    print("Array cleared")
    return jsonify({"status": "success", "message": "Array cleared successfully"})

@app.get("/get_pedal_board")
def get_pedal_board():
    current_pedal_array = get_state("pedal_array", [])
    return jsonify({"pedal_board": current_pedal_array})

@app.get("/test")
def test():
    print("get request received correctly")
    return jsonify({"status": "success"})

@app.errorhandler(404)
def page_not_found(error):
    return '<h1>404</h1>'

@app.errorhandler(400)
def page_not_found(error):
    return '<h1>400</h1>'

def shutdown_handler():
    # Retrieve the current state from the database
    pedal_array = get_state("pedal_array", [])
    curr_pedal = get_state("curr_pedal", 0)
    # Send the MIDI 'off' command to the current pedal
    if len(pedal_array) > 0:
        os.system('sendmidi dev "IAC Driver Bus 1" off ' + str(pedal_array[curr_pedal]) + ' 60')
    print("\nServer shutting down. Current pedal turned off.")

# This will ensure the shutdown_handler runs when the program exits normally.
atexit.register(shutdown_handler)

# These signal handlers catch Ctrl-C (SIGINT) and termination (SIGTERM) signals.
def handle_exit_signal(signum, frame):
    sys.exit(0)  # Trigger a clean exit (which calls atexit handlers)

signal.signal(signal.SIGINT, handle_exit_signal)
signal.signal(signal.SIGTERM, handle_exit_signal)

if __name__ == '__main__':
    app.run(debug=True)