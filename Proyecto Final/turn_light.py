# ===============================================================
# Author: Rodolfo Ferro Pérez
# Email: ferro@cimat.mx
# Twitter: @FerroRodolfo
#
# Script: GET requests to turn light on/off in WiFi module.
#
# ABOUT COPYING OR USING PARTIAL INFORMATION:
# This script was originally created by Rodolfo Ferro. Any
# explicit usage of this script or its contents is granted
# according to the license provided and its conditions.
# ===============================================================

import argparse
import requests


def parser():
    """Argument parser function."""
    # Construct the argument parser:
    ap = argparse.ArgumentParser()
    ap.add_argument("-u", "--url", required=True,
                    help="URL to WiFi module.")
    ap.add_argument("-s", "--state", type=str,
                    default="OFF", help="Light state.")
    args = vars(ap.parse_args())
    return args['url'], args['state']


def turn_ligth(url, state):
    """Function to turn state of light."""
    state = "LED={}".format(state)
    r = requests.get(url + state)
    return r.status_code


if __name__ == "__main__":
    url, state = parser()
    code = turn_ligth(url, state)
