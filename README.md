# SportsBroadSystem-TrackingVideo
> This is a framework that tracks the frames of zoom-video. It is based on the video stitching algorithm

**Video address** [link (version.1)](https://youtu.be/ZPp42GuiP0E) , [link (version.2)](https://youtu.be/ORXgAsRkWGU) 



![alt text](https://github.com/wjy5446/SportsBroadSystem-TrackingVideo/blob/master/image/abstract.png)



## 1. Overall Method

![head](https://github.com/wjy5446/Real-time-video-stitching/blob/master/image/flow-chart.png)

- Two Wide Videos are stitched using **SIFT, FLANN, RANSAC** for the total video.
- Spatial relationship between total video and zooming videos are computed using **Homography**
- **Homography validation** is used in the algorithm, Estimating the homography is often failed.
- **Kalman filtering** is used for removing the noise.



## 2. Homography validation

![head](https://github.com/wjy5446/Real-time-video-stitching/blob/master/image/mis_homography.png)

- The above figure represent results of misleading homograpy.

- So, we detect misleading homography using the validation equation

   

![head](https://github.com/wjy5446/Real-time-video-stitching/blob/master/image/template_matching.png)

- When find misleading homography, we use translation vector using block matching.



## 3. Kalman filtering

![head](https://github.com/wjy5446/Real-time-video-stitching/blob/master/image/kalman.png)

- We remove the noise of video-paths 



## 4. Result

![head](https://github.com/wjy5446/Real-time-video-stitching/blob/master/image/MFC.png)

- It was implemented in MFC.



## Thanks to



