from flask import Flask, render_template, request, redirect, url_for, abort
import json
import os
import time

app = Flask(__name__)

pedalboard = [0, 3, 7]
curr_pedal = 0
os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedalboard[curr_pedal]) + ' 60')

@app.get("/")
def index():
    return render_template("index.html", title = 'index')

@app.get("/next_pedal")
def next_pedal():
    global pedalboard
    global curr_pedal

    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedalboard[curr_pedal]) + ' 60')
    curr_pedal = (curr_pedal + 1) % len(pedalboard)
    time.sleep(1)
    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedalboard[curr_pedal]) + ' 60')
    return redirect(url_for('index'))

@app.get("/pedal_state")
def pedal_state():
    os.system('sendmidi dev "IAC Driver Bus 1" on ' + str(pedalboard[curr_pedal]) + ' 60')
    return redirect(url_for('index'))

@app.post("/pedal_value")
def pedal_value():
    r_json = request.json
    return redirect(url_for('index'))

@app.post("/pedalboard")
def pedal_board():
    r_json = request.json
    return redirect(url_for('index'))

@app.errorhandler(404)
def page_not_found(error):
    return '<h1>404</h1>'

@app.errorhandler(400)
def page_not_found(error):
    return '<h1>400</h1>'