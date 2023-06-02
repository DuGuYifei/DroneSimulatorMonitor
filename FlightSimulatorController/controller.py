from time import sleep

import airsim
import numpy as np
import win32api
import os
import cv2

client = airsim.MultirotorClient()
client.confirmConnection()

unlock = False
while True:
    x_forward = 0
    y_right = 0
    z_down = 0
    yaw_rate = 0

    responses = client.simGetImages(
        [
            airsim.ImageRequest(0, airsim.ImageType.Scene, False, False),
            airsim.ImageRequest(1, airsim.ImageType.Scene, False, False),
            airsim.ImageRequest(2, airsim.ImageType.Scene, False, False),
            airsim.ImageRequest(3, airsim.ImageType.Scene, False, False),
            airsim.ImageRequest(4, airsim.ImageType.Scene, False, False)
        ]
    )

    for i in range(len(responses)):
        response = responses[i]
        # get numpy array
        img1d = np.frombuffer(response.image_data_uint8, dtype=np.uint8)
        # reshape array to 4 channel image array H X W X 4
        img_rgb = img1d.reshape((response.height, response.width, 3))
        # # original image is fliped vertically
        # img_rgb = np.flipud(img_rgb)
        # write to png
        airsim.write_png(os.path.normpath(str(i) + '.png'), img_rgb)

    image = cv2.imread('1.png')
    # 显示图像
    cv2.imshow('Image', image)
    cv2.waitKey(1)
    # escape
    if (win32api.GetAsyncKeyState(0x1B) & 0x8000) > 0:
        break

    if (win32api.GetAsyncKeyState(0x4B) & 0x8000) > 0:
        # get control
        client.enableApiControl(True)
        print("get control")
        # unlock
        client.armDisarm(True)
        print("unlock")
        client.takeoffAsync().join()
        print("takeoff")
        unlock = True
        continue
    elif (win32api.GetAsyncKeyState(0x4C) & 0x8000) > 0:
        # land
        client.landAsync().join()
        print("land")
        # lock
        client.armDisarm(False)
        print("lock")
        # release control
        client.enableApiControl(False)
        print("release control")
        unlock = False
        continue

    if unlock:
        if (win32api.GetAsyncKeyState(0x57) & 0x8000) > 0:
            print("forward")
            x_forward = 20
        elif (win32api.GetAsyncKeyState(0x53) & 0x8000) > 0:
            # backward
            x_forward = -20

        if (win32api.GetAsyncKeyState(0x41) & 0x8000) > 0:
            # left
            y_right = -20
        elif (win32api.GetAsyncKeyState(0x44) & 0x8000) > 0:
            # right
            y_right = 20

        if (win32api.GetAsyncKeyState(0x26) & 0x8000) > 0:
            # up
            z_down = -20
        elif (win32api.GetAsyncKeyState(0x28) & 0x8000) > 0:
            # down
            z_down = 20

        if (win32api.GetAsyncKeyState(0x25) & 0x8000) > 0:
            # left rotate
            yaw_rate = -50
        elif (win32api.GetAsyncKeyState(0x27) & 0x8000) > 0:
            # right rotate
            yaw_rate = 50

        if x_forward != 0 or y_right != 0 or z_down != 0 or yaw_rate != 0:
            print("x_forward: ", x_forward, " y_right: ", y_right, " z_down: ", z_down, " yaw_rate: ", yaw_rate)
            client.moveByVelocityBodyFrameAsync(x_forward, y_right, z_down, 0.1,
                                                yaw_mode=airsim.YawMode(True, yaw_rate)).join()
        else:
            client.moveByVelocityAsync(0, 0, 0, duration=0.1).join()
            client.hoverAsync().join()
