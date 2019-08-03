#!/usr/bin/python3

for i in range(1, 8):  # col
    for j in range(1, 10):  # row
        tag_id = str((i-1)*9+(j-1))
        x_offset = round(.93*(j-1), 4)
        y_offset = -round(.93*(i-1), 4)
        print(
            '''<node pkg="tf2_ros" type="static_transform_publisher" name=\"map_tag_{tag_id}\" args=\"{x_offset} {y_offset} 0 0 0 0 map tag_{tag_id}\" />'''
            .format(tag_id=tag_id, x_offset=x_offset, y_offset=y_offset))
