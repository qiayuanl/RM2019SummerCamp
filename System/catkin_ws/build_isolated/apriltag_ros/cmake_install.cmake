# Install script for directory: /home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
        file(MAKE_DIRECTORY "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
      endif()
      if (NOT EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin")
        file(WRITE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/.catkin" "")
      endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/_setup_util.py")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated" TYPE PROGRAM FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/_setup_util.py")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/env.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated" TYPE PROGRAM FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/env.sh")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/setup.bash;/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/local_setup.bash")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated" TYPE FILE FILES
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/setup.bash"
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/local_setup.bash"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/setup.sh;/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/local_setup.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated" TYPE FILE FILES
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/setup.sh"
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/local_setup.sh"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/setup.zsh;/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/local_setup.zsh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated" TYPE FILE FILES
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/setup.zsh"
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/local_setup.zsh"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated/.rosinstall")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/install_isolated" TYPE FILE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/.rosinstall")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros/msg" TYPE FILE FILES
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/msg/AprilTagDetection.msg"
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/msg/AprilTagDetectionArray.msg"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros/srv" TYPE FILE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/srv/AnalyzeSingleImage.srv")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros/cmake" TYPE FILE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/apriltag_ros-msg-paths.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/include/apriltag_ros")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/share/roseus/ros/apriltag_ros")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/share/common-lisp/ros/apriltag_ros")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/share/gennodejs/ros/apriltag_ros")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  execute_process(COMMAND "/usr/bin/python" -m compileall "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/python2.7/dist-packages/apriltag_ros")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/python2.7/dist-packages/apriltag_ros")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/apriltag_ros.pc")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros/cmake" TYPE FILE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/apriltag_ros-msg-extras.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros/cmake" TYPE FILE FILES
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/apriltag_rosConfig.cmake"
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/catkin_generated/installspace/apriltag_rosConfig-version.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros" TYPE FILE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/package.xml")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/apriltag_ros" TYPE DIRECTORY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/include/apriltag_ros/" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros" TYPE DIRECTORY FILES
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/config"
    "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/launch"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/apriltag_ros" TYPE FILE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/nodelet_plugins.xml")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_common.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_common.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_common.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/libapriltag_ros_common.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_common.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_common.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_common.so"
         OLD_RPATH "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag/lib:/opt/ros/kinetic/lib:/opt/ros/kinetic/lib/x86_64-linux-gnu:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_common.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_continuous_detector.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_continuous_detector.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_continuous_detector.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/libapriltag_ros_continuous_detector.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_continuous_detector.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_continuous_detector.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_continuous_detector.so"
         OLD_RPATH "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag/lib:/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib:/opt/ros/kinetic/lib:/opt/ros/kinetic/lib/x86_64-linux-gnu:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_continuous_detector.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_continuous_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_continuous_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_continuous_node"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros" TYPE EXECUTABLE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/apriltag_ros/apriltag_ros_continuous_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_continuous_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_continuous_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_continuous_node"
         OLD_RPATH "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag/lib:/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib:/opt/ros/kinetic/lib:/opt/ros/kinetic/lib/x86_64-linux-gnu:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_continuous_node")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_client_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_client_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_client_node"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros" TYPE EXECUTABLE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/apriltag_ros/apriltag_ros_single_image_client_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_client_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_client_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_client_node"
         OLD_RPATH "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag/lib:/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib:/opt/ros/kinetic/lib:/opt/ros/kinetic/lib/x86_64-linux-gnu:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_client_node")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_single_image_detector.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_single_image_detector.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_single_image_detector.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/libapriltag_ros_single_image_detector.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_single_image_detector.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_single_image_detector.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_single_image_detector.so"
         OLD_RPATH "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag/lib:/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib:/opt/ros/kinetic/lib:/opt/ros/kinetic/lib/x86_64-linux-gnu:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libapriltag_ros_single_image_detector.so")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_server_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_server_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_server_node"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros" TYPE EXECUTABLE FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib/apriltag_ros/apriltag_ros_single_image_server_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_server_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_server_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_server_node"
         OLD_RPATH "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag/lib:/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/devel_isolated/apriltag_ros/lib:/opt/ros/kinetic/lib:/opt/ros/kinetic/lib/x86_64-linux-gnu:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros/apriltag_ros_single_image_server_node")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/apriltag_ros" TYPE PROGRAM FILES "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/src/apriltag_ros/apriltag_ros/scripts/analyze_image")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/gtest/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/qiayuanliao/Project/RM2019SummerCamp/System/catkin_ws/build_isolated/apriltag_ros/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
