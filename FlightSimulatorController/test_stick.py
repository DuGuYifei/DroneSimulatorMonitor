import pygame

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(1)
joystick.init()
print(joystick.get_init())
print(joystick.get_id())
while True:
    left_stick_x = joystick.get_axis(0)
    left_stick_y = joystick.get_axis(1)
    right_stick_x = joystick.get_axis(2)
    right_stick_y = joystick.get_axis(3)

    button_0 = joystick.get_button(0)
    button_1 = joystick.get_button(1)
    button_2 = joystick.get_button(2)
    button_3 = joystick.get_button(3)
    button_4 = joystick.get_button(4)
    button_5 = joystick.get_button(5)
    button_6 = joystick.get_button(6)
    button_7 = joystick.get_button(7)
    button_8 = joystick.get_button(8)
    button_9 = joystick.get_button(9)
    button_10 = joystick.get_button(10)

    if left_stick_x:
        print("left_stick_x: ", left_stick_x)
    if left_stick_y != 0.0:
        print("left_stick_y: ", left_stick_y)
    if right_stick_x != 0.0:
        print("right_stick_x: ", right_stick_x)
    if right_stick_y != 0.0:
        print("right_stick_y: ", right_stick_y)

    if button_0:
        print("button_0: ", button_0)
    if button_1:
        print("button_1: ", button_1)
    if button_2:
        print("button_2: ", button_2)
    if button_3:
        print("button_3: ", button_3)
    if button_4:
        print("button_4: ", button_4)
    if button_5:
        print("button_5: ", button_5)
    if button_6:
        print("button_6: ", button_6)
    if button_7:
        print("button_7: ", button_7)
    if button_8:
        print("button_8: ", button_8)
    if button_9:
        print("button_9: ", button_9)
    if button_10:
        print("button_10: ", button_10)

