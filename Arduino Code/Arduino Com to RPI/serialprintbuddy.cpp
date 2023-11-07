import cv2
import numpy as np
import pyrealsense2 as rs

# Configure the pipeline
pipeline = rs.pipeline()
config = rs.config()
config.enable_stream(rs.stream.color, 640, 480, rs.format.rgb8, 30)
config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
config.enable_stream(rs.stream.gyro)
config.enable_stream(rs.stream.accel)

# Start the pipeline
pipeline.start(config)

try:
    while True:
        # Wait for the next frameset
        frames = pipeline.wait_for_frames()

        # Get the color, depth, gyro, and accel frames
        color_frame = frames.get_color_frame()
        depth_frame = frames.get_depth_frame()
        gyro_frame = frames.first_or_default(rs.stream.gyro)
        accel_frame = frames.first_or_default(rs.stream.accel)

        if not color_frame or not depth_frame or not gyro_frame or not accel_frame:
            continue
            
        # Convert the color and depth images to numpy arrays
        color_image = np.asanyarray(color_frame.get_data())
        depth_image = np.asanyarray(depth_frame.get_data())

        # Convert the color image from BGR to RGB
        color_image = cv2.cvtColor(color_image, cv2.COLOR_BGR2RGB)
        
        # Access the IMU data
        gyro_data = gyro_frame.as_motion_frame().get_motion_data()
        accel_data = accel_frame.as_motion_frame().get_motion_data()

        # Display the color image
        cv2.imshow('Color Image', color_image)

        # Apply color map to depth image for visualization
        depth_colormap = cv2.applyColorMap(cv2.convertScaleAbs(depth_image, alpha=0.03), cv2.COLORMAP_JET)
        _, depth_jpeg = cv2.imencode('.jpg', depth_colormap)
        np_arr = np.frombuffer(depth_jpeg, np.uint8) # decode jpeg image type
        cv_depth_image = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)
        cv2.imshow('Depth Image', cv_depth_image)
        
        # Print the IMU data
        print(f"Gyro data: {gyro_data} | Type: {type(gyro_data)}")
        print(f"Accel data: {accel_data} | Type: {type(accel_data)}")

        # Break the loop if the 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    # Stop the pipeline
    pipeline.stop()

# Close OpenCV windows
cv2.destroyAllWindows()