#!/usr/bin/env python3

# Import base ROS
import rospy

# Import OpenCV and NumPy
import numpy as np

# Import ROS message information
from std_msgs.msg import Float32MultiArray
from std_msgs.msg import Float32
from sensor_msgs.msg import LaserScan

# Import dynamic reconfigure 
from dynamic_reconfigure.server import Server
from igvc_waypoint_nav.cfg import WaypointNavDynCfgConfig


ACTIVE_WINDOWS = []

##################################
# WaypointNavNode class definition
##################################
class WaypointNavNode():
    def __init__(self):
        """Waypoint navigation node"""

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

        # Define the image subscriber
        self.sub_lidar = rospy.Subscriber('/scan', LaserScan,
                                          self.lidar_callback)
                                          
        # Set up dynamic reconfigure
        self.srv = Server(WaypointNavDynCfgConfig,
                          self.dyn_reconfig_callback)
        # Define ROS rate
        self.rate = rospy.Rate(20)  # Vehicle rate

        # Loop
        while not rospy.is_shutdown():
            
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

        # Add configurations for image
        self.contour_area = config['cont_area']
        self.mask_top = config['mask_top']
        self.mask_right = config['mask_right']
        self.targ_line = config['targ_line']

        # Add calibration values
        self.test_length = config['test_length']
        self.test_time = config['test_time']
        self.track_width = config['track_width']
        self.wheel_radius = config['wheel_radius']
        
        self.dyn_config = config
        
        return config
    
    def lidar_callback(self, scan_msg):
        rospy.loginfo(scan_msg)
        return
    
#################    
# Main function
#################
if __name__ == '__main__':
    
    # Initialize the node and name it.
    rospy.init_node('waypoint_nav_node')
    print("Waypoint Navigation node initialized")
    
    # Start node
    try:
        WaypointNavNode()
    except rospy.ROSInterruptException:
        pass
