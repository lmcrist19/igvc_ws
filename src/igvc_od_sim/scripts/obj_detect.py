#!/usr/bin/env python3

# Import base ROS and Numpy
import rospy
import numpy as np

# Import os
import os

# Import ROS message information
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan

# Import dynamic reconfigure 
from dynamic_reconfigure.server import Server
from igvc_od_sim.cfg import ObjDetectDynCfgConfig

#################################
# ObjDetectNode class definition
#################################
class ObjDetectNode():
    def __init__(self):
        """Object detection node""" 

        # Define joystick subscriber
        self.sub_lidar = rospy.Subscriber('scan', LaserScan,
        				                  self.lidar_callback)
        # Define twist publisher
        self.pub_twist = rospy.Publisher('cmd_vel',
                                         Twist, queue_size=1)

        self.srv = Server(ObjDetectDynCfgConfig,
                          self.dyn_reconfig_callback)

        # Define ROS rate
        self.rate = rospy.Rate(20)

        # Loop and publish commands to vehicle
        while not rospy.is_shutdown():
            if self.enable:
                return
            # Sleep for time step
            self.rate.sleep()
        
        return

    ################################
    # Dynamic Reconfigure callback
    ################################
    def dyn_reconfig_callback(self, config, level):
        self.enable = config['enable']
        self.record_max = config['record_max']
        self.window = config['window']
        self.gain = config['gain']
        
        self.dyn_config = config
        
        return config
    
    #####################
    # LiDAR Callback
    #####################
    def lidar_callabck(self, scan_msg):
        max_angle = 360
        min_angle = 0
        
        front_angle = min_angle
        rear_angle = max_angle//2
        
        front_window_min = max_angle - self.window
        front_window_max = front_angle + self.window
        rear_window_min = rear_angle - self.window
        rear_window_max = rear_angle + self.window
        
        front_range = list(scan_msg.ranges[front_window_min:max_angle])
        for x in scan_msg.ranges[front_angle:front_window_max+1]:
            front_range.append(x)
        np.array(front_range)
        front_min_dist = np.amin(front_range)
        
        rear_range = list(scan_msg.ranges[rear_window_min:rear_window_max + 1])
        np.array(rear_range)
        rear_min_dist = np.amin(rear_range)
        
        self.position_err = front_min_dist - rear_min_dist
        
        # right scan_msg.ranges[0]
        # left scan_msg.ranges[180]
        
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
