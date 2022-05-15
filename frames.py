import cv2
import os


folder = "/home/scyber/data/videos/s18/"
file_list= os.listdir(folder)
for file_name in file_list:
	print(file_name)
	os.chdir("/home/scyber/data/s18_frames")
	new_folder= (file_name[:-4])
	os.mkdir(new_folder)
	os.chdir(new_folder)
	vidcap = cv2.VideoCapture(folder+file_name)
	count = 0
	while True:
		#print("gfhjkfgchhjcfghj"*10)
		success,image = vidcap.read()
		if ((success)and(count%5==0)):
			cv2.imwrite("frame%d.jpg" % count, image)     # save frame as JPEG file      
		if not(success):
			print("break")
			break
		print('Read a new frame in '+file_name+': ', success)
		count += 1
