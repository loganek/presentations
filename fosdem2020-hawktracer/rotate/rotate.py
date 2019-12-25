import cv2
import imutils
import numpy

from hawktracer.core import trace

# set env HT_PYTHON_TRACE_ENABLED=1 to enable tracing

@trace
def load_image(path):
    return cv2.imread(path)

@trace
def save_image(path, image):
    cv2.imwrite(path, image)

@trace
def rotate_image(image, angle):
    return imutils.rotate(image, angle)

@trace
def main(path, pattern):
    print(f'Rotating image {path} to {pattern}')
    image = load_image(path)
    for angle in numpy.arange(0, 360):
        rotated = rotate_image(image, angle)
        save_image(pattern.format(angle), rotated)
