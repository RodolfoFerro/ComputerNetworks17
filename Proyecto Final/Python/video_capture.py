# ===============================================================
# Author: Rodolfo Ferro Pérez
# Email: ferro@cimat.mx
# Twitter: @FerroRodolfo
#
# Script: Video display script with two resolution sizes.
#
# ABOUT COPYING OR USING PARTIAL INFORMATION:
# This script was originally created by Rodolfo Ferro. Any
# explicit usage of this script or its contents is granted
# according to the license provided and its conditions.
# ===============================================================

"""

# ===============================================================
# Author: Rodolfo Ferro Pérez
# Email: ferro@cimat.mx
# Twitter: @FerroRodolfo
#
# Script: Video display script with two resolution sizes.
#
# ABOUT COPYING OR USING PARTIAL INFORMATION:
# This script was originally created by Rodolfo Ferro. Any
# explicit usage of this script or its contents is granted
# according to the license provided and its conditions.
# ===============================================================

INSTRUCTIONS:

Over de cam screen...
1. Press <SPACEBAR> to change resolution.
2. Press <ESC> to close the camera.

"""

import numpy as np
import argparse
import imutils
import cv2


def resolution(cap, res=0):
    if res:
        cap.set(3, 640)
        cap.set(4, 480)
        return
    cap.set(3, 320)
    cap.set(4, 240)
    return


print(__doc__)
res, size = 0, ["320x240", "640x480"]
cap = cv2.VideoCapture(0)
resolution(cap, res)

while(True):
    # Capture frame-by-frame:
    ret, frame = cap.read()

    # Display the resulting frame:
    cv2.imshow('Camera w/resolution {}'.format(size[res]), frame)

    # Look for pressed key:
    key = cv2.waitKey(10)
    if key == 27:
        break
    if key == 32:
        res += 1
        res = res % 2
        cv2.destroyWindow('Camera w/resolution {}'.format(size[(res + 1) % 2]))
        resolution(cap, res)

# When everything done, release the capture:
cap.release()
cv2.destroyAllWindows()
