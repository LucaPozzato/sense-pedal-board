from flask import Flask, render_template, request, redirect, url_for, abort
import json

app = Flask(__name__)

pedal_board = []
curr_pedal = 0

@app.get("/")
def index():
    return render_template("index©.html", title = 'index')

@app.get("/next_pedal")
def next_pedal():
    return redirect(url_for('index'))

@app.get("/pedal_state")
def pedal_state():
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