import rospy
from geometry_msgs.msg import Point32
from geometry_msgs.msg import Twist
import math

f = 480/(2*math.tan(43/2))
def callback(data):
    #rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)
    x=data.x
    y=data.y
    d=data.z
    
    if d>0:
        x= x-320
        y=y-240
        yw = d*y/f
        xw = d*x/f
        angle = math.atan(xw/(d+20))*180/(math.pi)
        talker(angle)
    


def talker(angle):
    pub = rospy.Publisher('chatter', Twist, queue_size=10)
    rate = rospy.Rate(10) # 10hz
    message= Twist()
    message.linear.y=angle
    rospy.loginfo(message)
    pub.publish(message)
    rate.sleep()

def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("xy", Point32, callback)
    rospy.spin()

if __name__ == '__main__':
    listener()
    