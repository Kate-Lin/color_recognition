from collections import  deque
import numpy as np
#import imutils
import cv2
import time
import freenect


def find_object_with_kinect(colorLower, colorUpper, Object_Name):
    mybuffer = 64
    pts = deque(maxlen=mybuffer)
    frame,_ = freenect.sync_get_video()
    frame=cv2.cvtColor(frame,cv2.COLOR_RGB2BGR)
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, colorLower, colorUpper)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None
    if len(cnts) > 0:
        c = max(cnts, key = cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"]/M["m00"]), int(M["m01"]/M["m00"]))
        if radius > 10:
            rect=cv2.minAreaRect(c)
            cv2.rectangle(frame,(int(x)-int(radius),int(y)-int(radius)),(int(x+radius),int(y+2*radius)),(0,255,0),1)
            font = cv2.FONT_HERSHEY_SIMPLEX  
            cv2.putText(frame,Object_Name, (int(x)-int(radius),int(y)-int(radius)), font, 1, (255,255,255), 2)  
            pts.appendleft(center)
    cv2.imwrite("result.jpg",frame)

if __name__=="__main__":
    find_object_with_kinect(np.array([161,60,58]),np.array([218,255,255]),"Object_Name")