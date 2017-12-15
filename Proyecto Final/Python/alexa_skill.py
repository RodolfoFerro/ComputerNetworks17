# ===============================================================
# Author: Rodolfo Ferro Pérez
# Email: ferro@cimat.mx
# Twitter: @FerroRodolfo
#
# Script: Alexa skill to turn on/off LED (IoT).
#
# ABOUT COPYING OR USING PARTIAL INFORMATION:
# This script was originally created by Rodolfo Ferro. Any
# explicit usage of this script or its contents is granted
# according to the license provided and its conditions.
# ===============================================================

from flask_ask import Ask, statement, question, session
from turn_light import parser, turn_ligth
from flask import Flask
import requests
import logging

# Set global variables
name = 'Rodolfo'
url, state = None, None

# Set logger:
log = logging.getLogger()
log.addHandler(logging.StreamHandler())
log.setLevel(logging.DEBUG)
logging.getLogger("flask_ask").setLevel(logging.DEBUG)

# Create Flask, Ask apps:
app = Flask(__name__)  # Standard Flask app
ask = Ask(app, "/turn_light")  # App endpoint

# Initialize state (OFF):
url, state = parser()
state = "OFF"
anti_state = "ON"


# App route:
@app.route("/")
def homepage():
    return "Hey there {}! Flask is running with no problems!".format(name)


# Alexa initial message (starting app...):
@ask.launch
def start_skill():
    init_msg = "Hi! The light is {}. Would you like to turn it {}?".format(
        state, anti_state)
    return question(init_msg)


# If answer is yes:
@ask.intent("YesIntent")
def share_headlines():
    global url, state, anti_state
    code = turn_ligth(url, anti_state)
    state, anti_state = anti_state, state
    headline_msg = "Okay, the light has been turned {}.".format(state)
    return statement(headline_msg)


# If answer is no:
@ask.intent("NoIntent")
def no_intent():
    bye_text = "Okay, good bye."
    return statement(bye_text)


# End session
@ask.session_ended
def session_ended():
    log.debug("Session ended.")
    return "", 200


if __name__ == "__main__":
    app.run(debug=True)
