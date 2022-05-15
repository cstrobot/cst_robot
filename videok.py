# Python program to save a
# video using OpenCV

import freenect
import cv2


# Create an object to read
# from camera
#video = cv2.VideoCapture(2)

def get_video():
    array,_ = freenect.sync_get_video()
    array = cv2.cvtColor(array,cv2.COLOR_RGB2BGR)
    return array
    
i=input("person+iteration")
result = cv2.VideoWriter(i+'.avi', cv2.VideoWriter_fourcc(*'MJPG'),10, (640,480))

while(True):
	video = get_video()
	result.write(video)
	cv2.imshow('Frame', video)

	if cv2.waitKey(1) & 0xFF == ord('s'):
		break

result.release()
	
# Closes all the frames
cv2.destroyAllWindows()

print("The video was successfully saved")

