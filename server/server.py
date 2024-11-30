from flask import Flask, render_template, request, redirect, url_for, abort, jsonify
import numpy as np
import os
import time

app = Flask(__name__)

pedal_array = [0, 1, 2]
curr_pedal = 0
buffer = []
interpolated_points = [] 

os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedal_array[curr_pedal]) + ' 60')

@app.get("/")
def index():
    return render_template("index.html", title = 'index')

@app.get("/next_pedal")
def next_pedal():
    global pedal_array
    global curr_pedal

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

    print(value)

    buffer.append(value)

    # If we have at least 3 values, interpolate
    if len(buffer) == 3:
        # Interpolate 20 points
        new_points = interpolate_points(buffer)
        interpolated_points = new_points

        # Slide the buffer: Keep the last 2 values and wait for the next value
        buffer = buffer[-2:]

    for point in interpolated_points:
        os.system('sendmidi dev "IAC Driver Bus 1" cc ' + str(point) + ' ' + str(point))
        time.sleep(0.1)
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

def interpolate_points(points, num=20):
    x = [0, 1, 2]  # Fixed x-coordinates for the three points
    y = points
    x_new = np.linspace(0, 2, num=num + 2)  # Generate intermediate x-coordinates
    interpolated = np.interp(x_new, x, y)
    return interpolated[1:-1]  # Exclude the original points (start and end)