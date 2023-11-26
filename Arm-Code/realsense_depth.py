import cv2
import pyrealsense2 as rs
import numpy as np

class DepthCamera:
    DEPTH_WIDTH = 640
    DEPTH_HEIGHT = 480
    COLOR_WIDTH = 640
    COLOR_HEIGHT = 480
    INFRARED_WIDTH = 640
    INFRARED_HEIGHT = 480

    def __init__(self):
        # Configure depth, color, and infrared streams
        self.pipeline = rs.pipeline()
        config = rs.config()

        config.enable_stream(rs.stream.depth, self.DEPTH_WIDTH, self.DEPTH_HEIGHT, rs.format.z16, 30)
        config.enable_stream(rs.stream.color, self.COLOR_WIDTH, self.COLOR_HEIGHT, rs.format.bgr8, 30)
        config.enable_stream(rs.stream.infrared, 1, self.INFRARED_WIDTH, self.INFRARED_HEIGHT, rs.format.y8, 30)
        config.enable_stream(rs.stream.infrared, 2, self.INFRARED_WIDTH, self.INFRARED_HEIGHT, rs.format.y8, 30)

        # Start the pipeline and get the active profile
        self.pipeline.start(config)

        # Additional settings for autoexposure
        #sensor = profile.get_device().first_color_sensor()
        #sensor.set_option(rs.option.enable_auto_exposure, 1)

        self.align = rs.align(rs.stream.color)

    def get_frame(self):
        frames = self.pipeline.wait_for_frames()
        aligned_frames = self.align.process(frames)
        aligned_depth_frame = aligned_frames.get_depth_frame()
        color_frame = aligned_frames.get_color_frame()
        infrared_frame = aligned_frames.get_infrared_frame(1)
        infrared_frame1 = aligned_frames.get_infrared_frame(2)

        if not aligned_depth_frame or not color_frame or not infrared_frame or not infrared_frame1:
            return False, None, None, None, None

        dtd_depth = self.dtd(aligned_depth_frame)
        hole_filled_depth_frame = self.hole_filling(dtd_depth)
        filtered_depth = self.spatial(hole_filled_depth_frame)

        depth_image = np.asanyarray(filtered_depth.get_data())
        color_image = np.asanyarray(color_frame.get_data())
        infrared_image = np.asanyarray(infrared_frame.get_data())
        infrared_image1 = np.asanyarray(infrared_frame1.get_data())

        return True, depth_image, color_image, infrared_image, infrared_image1

    def release(self):
        self.pipeline.stop()

    def hole_filling(self, depth_frame):
        hole_filling = rs.hole_filling_filter()
        return hole_filling.process(depth_frame)

    def spatial(self, depth_frame):
        spatial = rs.spatial_filter()
        spatial.set_option(rs.option.filter_magnitude, 2)
        spatial.set_option(rs.option.filter_smooth_alpha, 0.25)
        spatial.set_option(rs.option.filter_smooth_delta, 20)
        return spatial.process(depth_frame)

    def dtd(self, depth_frame):
        depth_to_disparity = rs.disparity_transform(False)
        return depth_to_disparity.process(depth_frame)

if __name__ == "__main__":
    dc = DepthCamera()
    while True:
        ret, depth_frame, color_frame, infrared_frame, infrared_frame1 = dc.get_frame()
        if not ret:
            break

        normalized_depth = cv2.normalize(depth_frame, None, 0, 255, cv2.NORM_MINMAX)
        normalized_depth = np.uint8(normalized_depth)
        colored_depth = cv2.applyColorMap(normalized_depth, cv2.COLORMAP_JET)
        alpha = 0.7
        overlay = cv2.addWeighted(color_frame, 1 - alpha, colored_depth, alpha, 0)

        cv2.imshow("Depth Overlaid on Color", overlay)

        key = cv2.waitKey(1)
        if key == 27:
            break

    dc.release()
    cv2.destroyAllWindows()
