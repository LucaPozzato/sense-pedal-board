from flask import Flask, render_template, request, redirect, url_for, abort, jsonify
import os
import time

app = Flask(__name__)

REQUEST_INTERVAL = 2
last_request_time = time.time()

pedal_array = [0]
curr_pedal = 0
buffer = []
interpolated_points = [] 

os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')

@app.get("/")
def index():
    return render_template("index.html", title = 'index')

@app.get("/next_pedal")
def next_pedal():
    global pedal_array, curr_pedal, last_request_time

    current_time = time.time()
    elapsed_time = current_time - last_request_time
    if elapsed_time > REQUEST_INTERVAL:
        last_request_time = current_time
        os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')
        curr_pedal = (curr_pedal + 1) % len(pedal_array)
        time.sleep(1)
        os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')

    return redirect(url_for('index'))

@app.get("/pedal_state")
def pedal_state():
    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')
    return redirect(url_for('index'))

@app.post("/pedal_value")
def pedal_value():
    global buffer, interpolated_points
    r_json = request.json
    value = r_json['pedal value']

    if int(value) == 0:
        scaled_value = 10
    else:
        scaled_value = int(value)/9*127
    
    print(scaled_value)

    print(value)

    os.system('sendmidi dev "IAC Driver Bus 1" cc ' + str(scaled_value) + ' ' + str(scaled_value))
    return redirect(url_for('index'))

@app.post("/add_pedal")
def add_pedal():
    global pedal_array

    # Parse the incoming JSON payload
    data = request.get_json()
    pedal_number = data.get("pedalNumber")

    if pedal_number is None:  # Validate that the number exists
        return jsonify({"status": "error", "message": "Pedal number is missing"}), 400

    # Append the pedal number to the array
    pedal_array.append(pedal_number)

    # Log for debugging (optional)
    print(f"Added pedal number: {pedal_number}")
    print(f"Current pedal array: {pedal_array}")

    # Return a success response
    return jsonify({"status": "success", "message": f"Pedal {pedal_number} added successfully"})


@app.get("/clear_pedal_array")
def clear_array():
    global pedal_array
    global curr_pedal
    
    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')
    pedal_array.clear()
    pedal_array.append(0)
    curr_pedal = 0
    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')
    print(f"Array cleared")
    print(pedal_array)
    return jsonify({"status": "success", "message": f"Array cleared successfully"})


@app.errorhandler(404)
def page_not_found(error):
    return '<h1>404</h1>'

@app.errorhandler(400)
def page_not_found(error):
    return '<h1>400</h1>'