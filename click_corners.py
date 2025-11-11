import cv2 as cv
import numpy as np
pts = []

img = cv.imread('frame.png')
disp = img.copy()

def on_mouse(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        if len(pts) < 4:
            pts.append((x,y))
            cv.circle(disp, (x,y), 5, (0,255,0), -1)
            cv.putText(disp, str(len(pts)), (x+8,y-8), cv.FONT_HERSHEY_SIMPLEX, 0.6, (0,255,0), 2)
            cv.imshow('click 4 corners (TL,TR,BR,BL)', disp)
        if len(pts) == 4:
            s = ';'.join([f'{u},{v}' for (u,v) in pts])
            print('\nimg-corners:', s)
            with open('img_corners.txt','w') as f: f.write(s+'\n')

cv.namedWindow('click 4 corners (TL,TR,BR,BL)', cv.WINDOW_NORMAL)
cv.imshow('click 4 corners (TL,TR,BR,BL)', disp)
cv.setMouseCallback('click 4 corners (TL,TR,BR,BL)', on_mouse)
print('Click the 4 table corners in this order: TL, TR, BR, BL. Press ESC if needed.')
while True:
    key = cv.waitKey(20)
    if key == 27: break
cv.destroyAllWindows()
