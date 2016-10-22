import cv2
from customTimer import RepeatedTimer
from time import sleep
import v4l2
import subprocess

# define list of resolution tuples in format (width,height)
resolutions = [(320, 240),
               (640, 480),
               (800, 600),
               (1024, 768),
               (1280, 720),
               (1280, 1024),
               (1600, 1200),
               (1920, 1080),
               (2048, 1536),
               (2592, 1944)]

frame = 0

def set_resolution(*args):
    vc.set(cv2.CAP_PROP_FRAME_WIDTH, args[0][0])   # set frame width in pixels
    vc.set(cv2.CAP_PROP_FRAME_HEIGHT, args[0][1])  # set frame height in pixels

def setup():
    set_resolution(resolutions[8])

    command = 'v4l2-ctl -d /dev/video0 -c brightness=13,exposure_auto=1,exposure_absolute=20'
    subprocess.call([command], shell=True)

def capture_frame():    
    capture_frame.count += 1    
    cv2.imwrite("frame%04d.jpg" % capture_frame.count, frame)
    print "captured frame " + str(capture_frame.count)

capture_frame.count = 0

if __name__ == '__main__':

    vc = cv2.VideoCapture(0)

    if vc.isOpened():
        # configure camera
        setup()
        
        print "Starting..."
        rt = RepeatedTimer(1, capture_frame)

        try:        
            while True:                       
                retval, frame = vc.read()
        except KeyboardInterrupt:
            print "loop interrupted!"        
        
        rt.stop()

        vc.release()
        cv2.destroyAllWindows()

    print "program exit"
