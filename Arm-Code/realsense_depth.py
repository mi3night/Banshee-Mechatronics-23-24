import cv2
import pyrealsense2 as rs
import numpy as np

# Add spatial and temporal filters to the pipeline configuration within the DepthCamera class
class DepthCamera:
    def __init__(self):
        # Configure depth and color streams
        self.pipeline = rs.pipeline()
        config = rs.config()

        # Get device product line for setting a supporting resolution
        pipeline_wrapper = rs.pipeline_wrapper(self.pipeline)
        pipeline_profile = config.resolve(pipeline_wrapper)
        device = pipeline_profile.get_device()
        device_product_line = str(device.get_info(rs.camera_info.product_line))

        config.enable_stream(rs.stream.depth, 640, 480, rs.format.z16, 30)
        config.enable_stream(rs.stream.color, 640, 480, rs.format.bgr8, 30)

        # Create an align object for aligning depth and color frames
        self.align = rs.align(rs.stream.color)

        # Start streaming
        self.pipeline.start(config)

        # Add the spatial and temporal filters to the pipeline configuration
        self.threshold_filter = rs.threshold_filter(0.3, 0.7)
        self.temp_filter = rs.temporal_filter(0.1, 80.0, 6)
        self.spat_filter = rs.spatial_filter(0.40, 40.0, 4.0, 5.0)  # test settings - 1st = aplha, 2nd = delta, 3rd = magniture, 4th 
        # = holefilling(0 = none, 1 = 2px, 2 = 4px, 3 = 8px, 4 = 16px, 5 = unlimited)


        
    def get_frame(self):
        frames = self.pipeline.wait_for_frames()

        # Align the frames to get the depth and color frames in sync
        aligned_frames = self.align.process(frames)
        aligned_depth_frame = aligned_frames.get_depth_frame()
        color_frame = aligned_frames.get_color_frame()
        depth_frame = frames.get_depth_frame()


        if not aligned_depth_frame or not color_frame:
            return False, None, None

        # Apply hole-filling filter


        # Convert frames to numpy arrays
        depth_image = np.asanyarray(depth_frame.get_data())
        color_image = np.asanyarray(color_frame.get_data())

        return True, depth_image, color_image

    def release(self):
        self.pipeline.stop()

    def hole_filling(self, depth_frame):
        hole_filling = rs.hole_filling_filter()
        return hole_filling.process(depth_frame)

# Example usage
if __name__ == "__main__":
    dc = DepthCamera()
    while True:
        ret, depth_frame, color_frame = dc.get_frame()
        if not ret:
            break

        # Normalize and apply a colormap to the depth frame
        normalized_depth = cv2.normalize(depth_frame, None, 0, 255, cv2.NORM_MINMAX)
        colored_depth = cv2.applyColorMap(normalized_depth, cv2.COLORMAP_JET)

        cv2.imshow("Colored Depth frame", colored_depth)
        cv2.imshow("Color frame", color_frame)

        key = cv2.waitKey(1)
        if key == 27:
            break

    dc.release()
    cv2.destroyAllWindows()
