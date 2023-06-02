import airsim
import pygame
import time

client = airsim.MultirotorClient()
client.confirmConnection()
unlock = False

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)
joystick.init()



while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            break

    left_stick_x = joystick.get_axis(0)
    left_stick_y = joystick.get_axis(1)
    right_stick_x = joystick.get_axis(2)
    right_stick_y = joystick.get_axis(3)

    # escape
    share_button = joystick.get_button(4)

    cross_button = joystick.get_button(0)
    # circle_button = joystick.get_button(1)
    # triangle_button = joystick.get_button(2)
    # a = joystick.get_button(3)
    # b = joystick.get_button(4)
    # c = joystick.get_button(5)
    # d = joystick.get_button(6)
    # e = joystick.get_button(7)

    x_forward = left_stick_x * 2.5
    y_right = left_stick_y * 2.5
    z_down = right_stick_x * -1
    yaw_rate = right_stick_y * 0.314

    # escape
    if share_button:
        break

    if not unlock and cross_button:
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

    if unlock:
        if cross_button:
            # land
            client.landAsync(timeout_sec=3).join()
            print("land")
            # lock
            client.armDisarm(False)
            print("lock")
            # release control
            client.enableApiControl(False)
            print("release control")
            unlock = False
            continue

        if x_forward != 0 or y_right != 0 or z_down != 0 or yaw_rate != 0:
            print("x_forward: ", x_forward, " y_right: ", y_right, " z_down: ", z_down, " yaw_rate: ", yaw_rate)
            client.moveByVelocityBodyFrameAsync(x_forward, y_right, z_down, 0.05,
                                                yaw_mode=airsim.YawMode(True, yaw_rate)).join()
            time.sleep(0.05)
        else:
            client.moveByVelocityAsync(0, 0, 0, duration=0.1).join()
            client.hoverAsync().join()
