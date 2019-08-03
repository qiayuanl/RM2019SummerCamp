#!/usr/bin/python3

# This script is used to generate the apriltag fullmap

for i in range(1, 8):  # col
    for j in range(1, 10):  # row
        tag_id = str((i-1)*9+(j-1)) # generate tag_id
        x_offset = round(.93*(7-i), 2) # round to 2 decimals
        y_offset = round(.93*(9-j), 2)
        print(
            '''<node pkg="tf2_ros" type="static_transform_publisher" name=\"map_tag_{tag_id}\" args=\"{x_offset} {y_offset} 0 0 0 0 map tag_{tag_id}\" />'''
            .format(tag_id=tag_id, x_offset=x_offset, y_offset=y_offset))
