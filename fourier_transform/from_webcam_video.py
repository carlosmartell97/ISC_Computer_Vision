import cv2
import numpy as np
from matplotlib import pyplot as plt
import sys


def grab_frame(cap):
    ret, frame = cap.read()
    return cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)


# Initiate the camera
cap = cv2.VideoCapture(0)

if not cap.open(0):
    print "webcam is not active! exiting now..."
    sys.exit()

# create two subplots
ax1 = plt.subplot(1, 2, 1)
plt.xticks([])
plt.yticks([])
ax2 = plt.subplot(1, 2, 2)

# create two image plots
im1 = ax1.imshow(grab_frame(cap), cmap='gray')
im2 = ax2.imshow(grab_frame(cap), cmap='gray')
plt.xticks([])
plt.yticks([])

plt.ion()

print "Starting. Press 'q' to quit."
while True:
    ret, frame = cap.read()
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    f = np.fft.fft2(gray_frame)
    fshift = np.fft.fftshift(f)
    magnitude_spectrum = 20*np.log(np.abs(fshift))

    im1.set_data(frame)
    im2.set_data(magnitude_spectrum)
    plt.pause(0.1)
plt.show()
