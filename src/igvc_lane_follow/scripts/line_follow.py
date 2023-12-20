#!/usr/bin/env python3

# Import base ROS
import rospy

# Import OpenCV and NumPy
import cv2 as cv
import numpy as np

# Import ROS message information
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Float32
from sensor_msgs.msg import Image
from cv_bridge import CvBridge

# Import dynamic reconfigure 
from dynamic_reconfigure.server import Server
from lane_follow_igvc.cfg import LineFollowDynCfgConfig


ACTIVE_WINDOWS = []

##################################
# LineFollowNode class definition
##################################
class LineFollowNode():
    def __init__(self):
        """Line following node"""

        # Initialize dynamic reconfigure
        self.enable = 0
        self.gain = 0.0
        self.speed = 0.0 # @1m/s, GAIN = 3.5
        self.frame_skip = 0
        self.hue_low = 0
        self.hue_high = 0
        self.sat_low = 0
        self.sat_high = 0
        self.val_low = 0
        self.val_high = 0
        self.contour_area = 0
        self.mask_top = 1
        self.mask_right = 1
        self.targ_line= 0
        self.test_length = 0
        self.test_time = 0
        self.track_width = 0
        self.wheel_radius = 0
        self.max_rpm = 0
        self.rpm2analog = 0
        self.valid_contours = 0
        self.dimension_arr = Float32MultiArray()

        # Initialize robot motion
        self.steer = 0.0

        # Define the image subscriber (Cam), comment out if using USB cam
        #self.image_view = rospy.Subscriber('usb_cam/image_raw', Image,
                                          #self.camera_callback, queue_size=1)

        # Define the image subscriber (Sim), comment out if using Gazelle Sim
        self.image_view = rospy.Subscriber('camera/image_raw', Image,
                                          self.camera_callback, queue_size=1)
        # Define cmd publisher
        self.pub_twist = rospy.Publisher('cmd_vel',
                                         Twist, queue_size=1)
        # Define arduino listener
        self.sub_arduino = rospy.Subscriber('comms/arduino', Float32MultiArray,
                                         self.arduino_callback, queue_size=1)
        #self.sub_arduino = rospy.Subscriber('comms/arduino', Float32,
                                         #self.arduino_callback, queue_size=1)
        # Define python talker
        self.pub_arduino = rospy.Publisher('comms/python', Float32MultiArray, queue_size=1)
                                                    
        # Set up dynamics reconfigure
        self.srv = Server(LineFollowDynCfgConfig,
                          self.dyn_reconfig_callback)
        # Define ROS rate
        self.rate = rospy.Rate(20)  # Vehicle rate

        # Loop and publish commands to vehicle
        while not rospy.is_shutdown():
            # Build message yaw and speed rate message and publish
            if self.enable:
                twist_msg.linear.x = self.speed
                twist_msg.angular.z = self.steer
            else:
                twist_msg = Twist()
            rotations = self.test_length/(2*np.pi*self.wheel_radius)
            self.max_rpm = (60*(rotations/self.test_time))
            self.rpm2analog = 255/self.max_rpm
            self.dimension_arr.data = [self.track_width, self.wheel_radius,
                                          self.max_rpm, self.rpm2analog] # assign the array with the value you want to send
            self.pub_arduino.publish(self.dimension_arr)
            self.pub_twist.publish(twist_msg)
            
            # Sleep for time step
            self.rate.sleep()
            
        return

    ################################
    # Dynamic Reconfigure callback
    ################################
    def dyn_reconfig_callback(self, config, level):
        self.enable = config['enable']
        self.gain = config['gain']

        # Add HSV hue, sat and val
        self.hue_low = config['hue_low']
        self.hue_high = config['hue_high']
        self.sat_low = config['sat_low']
        self.sat_high = config['sat_high']
        self.val_low = config['val_low']
        self.val_high = config['val_high']
        self.contour_area = config['cont_area']
        self.mask_top = config['mask_top']
        self.mask_right = config['mask_right']
        self.targ_line = config['targ_line']
        self.test_length = config['test_length']
        self.test_time = config['test_time']
        self.track_width = config['track_width']
        self.wheel_radius = config['wheel_radius']
        
        self.dyn_config = config
        
        return config

    #########################
    # Camera image callback
    #########################
    def camera_callback(self, rgb_msg):
        # Get the camera image and make a copy
        img = CvBridge().imgmsg_to_cv2(rgb_msg, "bgr8" )
        img_masked = img          
        rows, cols = img.shape[:2]
        start_point_top = (0,0)
        end_point_top = (cols,int(rows/self.mask_top))
        start_point_right = (int(cols/self.mask_right),0)
        end_point_right = (cols,rows)
        mask_color = (255,0,0)
        mask_top = cv.rectangle(img_masked, start_point_top, end_point_top, mask_color, -1)
        mask_right = cv.rectangle(img_masked, start_point_right, end_point_right, mask_color, -1)
        img_masked = cv.bitwise_and(img_masked,mask_top)
        img_masked = cv.bitwise_and(img_masked,mask_right)
        

        # Convert image to a HSV image and blur
        img_gray = cv.cvtColor(img_masked, cv.COLOR_BGR2GRAY)
        img_gray = cv.cvtColor(img_gray, cv.COLOR_GRAY2BGR)
        img_hsv = cv.cvtColor(img_gray, cv.COLOR_BGR2HSV)
        img_blur = cv.medianBlur(img_hsv, 7)
        
        # Threshold HSV image to binary based on range
        img_hsv_thresh = cv.inRange(img_blur,
                                   (self.hue_low, self.sat_low, self.val_low),
                                   (self.hue_high, self.sat_high, self.val_high))
        
        # Form and apply visible target line in frame
        targ_line = cols/self.targ_line
        targ_line_start, targ_line_end = (int(cols/self.targ_line), 0), (int(cols/self.targ_line), rows)
        

        # Find and store each contour in frame, display contours in frame
        self.contours, self.hierarchy = cv.findContours(img_hsv_thresh, cv.RETR_TREE,
                                               cv.CHAIN_APPROX_SIMPLE)
        
        
        for i, contour in enumerate(self.contours):
            self.cnt = self.contours[i]
            M = cv.moments(self.cnt)
            CG_x = M['m10']/M['m00'] if M['m00'] > 0 else 0
            CG_y = M['m01']/M['m00'] if M['m00'] > 0 else 0
            CG = np.array([CG_x, CG_y])
            
            if M['m00'] > self.contour_area:
                self.valid_contours += 1
                cv.circle(img, (int(CG[0]), int(CG[1])), 7, (255, 0, 0), -1)
                # Calculate error based on the center of the image and the gain
                err = targ_line - CG[0]
                p_control = self.gain * err/targ_line 
                self.speed = self.dyn_config['speed']
                self.steer = p_control
            else:
                cv.circle(img, (int(CG[0]), int(CG[1])), 7, (0, 0, 255), -1)

        # Set speed if valid contours are found
        if self.valid_contours < 1:
            self.speed = 0.0
            self.steer = 0.0
        else:
            self.speed = self.dyn_config['speed']
        # Decorate and display images
        self.valid_contours = 0      
        cv.line(img, targ_line_start, targ_line_end, (0,255,0), 3)
        cv.drawContours(img_hsv_thresh, self.contours, -1, (0,255,0), 10)
        self.display_image('Raw Image', img, True)
        self.display_image('Masked Image', img_masked, True)
        self.display_image('HSV Image Threshold', img_hsv_thresh, True)
        self.display_image('Centroid', img, True)
        return
    
    ########################
    # Arduino Comms Callback
    ########################
    def arduino_callback(self, data):
    # Access the data array inside Float32MultiArray
        dimension_data = data.data

    ####################
    # Display an image
    ####################
    def display_image(self, title_str, img, disp_flag ):

        if( disp_flag ):
            # Display the given image
            cv.namedWindow(title_str, cv.WINDOW_NORMAL)
            cv.imshow(title_str, img)
            cv.waitKey(3)

            # Add window to active window list
            if not ( title_str in ACTIVE_WINDOWS ):
                ACTIVE_WINDOWS.append(title_str)
        else:
            if( title_str in ACTIVE_WINDOWS):
                cv.destroyWindow(title_str)
                ACTIVE_WINDOWS.remove(title_str)
        return
    
#################    
# Main function
#################
if __name__ == '__main__':
    
    # Initialize the node and name it.
    rospy.init_node('line_folllow_node')
    print("Line Follow node initialized")
    
    # Start node
    try:
        LineFollowNode()
    except rospy.ROSInterruptException:
        pass
