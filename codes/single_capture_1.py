import cv2

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


def set_resolution(*args):

    vc.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, args[0][0])   # set frame width in pixels
    vc.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, args[0][1])  # set frame height in pixels


if __name__ == '__main__':

    vc = cv2.VideoCapture(0)

    if vc.isOpened():
        # set resolution
        set_resolution(resolutions[8])

        retval, frame = vc.read()
        cv2.imwrite("opencv_image.jpg", frame)

    vc.release()