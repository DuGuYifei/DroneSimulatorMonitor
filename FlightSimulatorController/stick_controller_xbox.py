import airsim
import pygame

client = airsim.MultirotorClient()
client.confirmConnection()
unlock = False
previous_cross_A = 0
previous_cross_B = 0

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(1)
joystick.init()

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            break

    # left stick - left/right -> move left/right
    left_stick_x = joystick.get_axis(0)
    if abs(left_stick_x) < 0.2:
        left_stick_x = 0
    # left stick - up/down -> move forward/backward
    left_stick_y = joystick.get_axis(1)
    if abs(left_stick_y) < 0.2:
        left_stick_y = 0
    # right stick - left/right -> yaw left/right
    right_stick_x = joystick.get_axis(2)
    if abs(right_stick_x) < 0.2:
        right_stick_x = 0
    # right stick - up/down -> move up/down
    right_stick_y = joystick.get_axis(3)
    if abs(right_stick_y) < 0.2:
        right_stick_y = 0

    # triangle - Y - escape game
    triangle_Y_escape = joystick.get_button(3)
    # cross - A - get/lock control
    cross_A_lock = joystick.get_button(0)
    if cross_A_lock != previous_cross_A:
        previous_cross_A = cross_A_lock
    else:
        cross_A_lock = 0

    # circle - B - rain/snow/nothing
    # circle_B_weather = joystick.get_button(1)
    # if circle_B_weather != previous_cross_B:
    #     previous_cross_B = circle_B_weather
    # else:
    #     circle_B_weather = 0

    x_forward = left_stick_y * -10
    y_right =  left_stick_x * 10
    z_down = right_stick_y * 50
    yaw_rate = right_stick_x * 90

    # escape
    if triangle_Y_escape:
        break

    if not unlock and cross_A_lock:
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
        if cross_A_lock:
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
        else:
            client.moveByVelocityAsync(0, 0, 0, duration=0.1).join()
            client.hoverAsync().join()
