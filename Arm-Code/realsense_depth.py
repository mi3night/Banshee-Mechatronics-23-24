import cv2
import pyrealsense2 as rs
import numpy as np

class DepthCamera:
    DEPTH_WIDTH = 1280
    DEPTH_HEIGHT = 720
    COLOR_WIDTH = 1920
    COLOR_HEIGHT = 1080

    def __init__(self):
        # Configure depth and color streams
        self.pipeline = rs.pipeline()
        config = rs.config()

        config.enable_stream(rs.stream.depth, self.DEPTH_WIDTH, self.DEPTH_HEIGHT, rs.format.z16, 30)
        config.enable_stream(rs.stream.color, self.COLOR_WIDTH, self.COLOR_HEIGHT, rs.format.bgr8, 30)

        self.align = rs.align(rs.stream.color)
        self.pipeline.start(config)

    def get_frame(self):
        frames = self.pipeline.wait_for_frames()
        aligned_frames = self.align.process(frames)
        aligned_depth_frame = aligned_frames.get_depth_frame()
        color_frame = aligned_frames.get_color_frame()

        if not aligned_depth_frame or not color_frame:
            return False, None, None
        

        dtd_depth = self.dtd(aligned_depth_frame)
        hole_filled_depth_frame = self.hole_filling(dtd_depth)
        filtered_depth = self.spatial(hole_filled_depth_frame)



        depth_image = np.asanyarray(filtered_depth.get_data())
        color_image = np.asanyarray(color_frame.get_data())

        return True, depth_image, color_image

    def release(self):
        self.pipeline.stop()

    def hole_filling(self, depth_frame):
        hole_filling = rs.hole_filling_filter(2)
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
        ret, depth_frame, color_frame = dc.get_frame()
        if not ret:
            break

        normalized_depth = cv2.normalize(depth_frame, None, 0, 255, cv2.NORM_MINMAX)
        colored_depth = cv2.applyColorMap(normalized_depth, cv2.COLORMAP_JET)
        alpha = 0.7
        overlay = cv2.addWeighted(color_frame, 1 - alpha, colored_depth, alpha, 0)

        cv2.imshow("Depth Overlaid on Color", overlay)

        key = cv2.waitKey(1)
        if key == 27:
            break

    dc.release()
    cv2.destroyAllWindows()
