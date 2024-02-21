#!/usr/bin/env python3

# Import base ROS
import rospy

# Import NumPy
import numpy as np

# Import ROS message information
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist

# Import dynamic reconfigure 
from dynamic_reconfigure.server import Server
from igvc_obj_detect.cfg import ObjDetectDynCfgConfig


ACTIVE_WINDOWS = []

##################################
# ObjDetectNode class definition
##################################
class ObjDetectNode():
    def __init__(self):
        """Object detection node"""

        # Initialize dynamic reconfigure
        self.enable = 0
        self.gain = 0.0
        self.speed = 0.0 # @1m/s, GAIN = 3.5
        self.window = 0

        # Define the subscriber
        self.sub_lidar = rospy.Subscriber('scan', LaserScan,
                                          self.lidar_callback)
        
        # Define publishers
        self.pub_twist = rospy.Publisher('cmd_vel',
                                         Twist, queue_size=1)
        self.pub_lidar = rospy.Publisher('scan',
 	                                     LaserScan, queue_size=1)
                                          
        # Set up dynamic reconfigure
        self.srv = Server(ObjDetectDynCfgConfig,
                          self.dyn_reconfig_callback)
        
        # Define ROS rate
        self.rate = rospy.Rate(20)  # Vehicle rate

        # Loop
        while not rospy.is_shutdown():
            if self.enable:
                rospy.loginfo(f'Enabled')
            else:
                twist_msg = Twist()
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
        self.window = config['window']
        self.window = config['speed']
        
        self.dyn_config = config
        
        return config
    
    ##################
    # Lidar callback
    ##################    
    def lidar_callback(self, scan_msg):
        rospy.loginfo(scan_msg.ranges)
        return
    
#################    
# Main function
#################
if __name__ == '__main__':
    
    # Initialize the node and name it.
    rospy.init_node('obj_detect_node')
    print("Object Detection node initialized")
    
    # Start node
    try:
        ObjDetectNode()
    except rospy.ROSInterruptException:
        pass
